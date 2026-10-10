import json
import re
import tempfile
from pathlib import Path
import unittest
import xml.etree.ElementTree as ET

from progress import (CODE, ORIGINAL_ASM, classify, code_family, generate, layout, load_original_asm, parse_map, svg,
                      union_size, source_coverage)


class ProgressTests(unittest.TestCase):
    def test_mismatched_rom_is_rejected_before_reporting(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'build/heartgold.us').mkdir(parents=True)
            (root / 'heartgold.us').mkdir()
            (root / 'build/heartgold.us/pokeheartgold.us.nds').write_bytes(b'wrong ROM')
            (root / 'heartgold.us/rom.sha1').write_text('0' * 40 + '  rom.nds')
            with self.assertRaisesRegex(ValueError, 'ROM mismatch'):
                generate(root, 'heartgold.us')

    def test_aliases_and_section_symbols_are_not_double_counted(self):
        self.assertEqual(union_size([(0, 100), (0, 20), (20, 60), (95, 110)]), 110)

    def test_overlay_addresses_stay_separate_and_spec_resolves_sources(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'src').mkdir()
            (root / 'asm').mkdir()
            (root / 'src/unit.c').write_text('void f(void) {}')
            (root / 'asm/unit.s').write_text('')
            (root / 'main.lsf').write_text('Object src/unit.o\n')
            path = root / 'test.xMAP'
            path.write_text('# .OVY_1\n'
                            '  02000000 00000020 .text .text (unit.o)\n'
                            '  02000000 00000010 .text f (unit.o)\n'
                            '  02000000 00000000 .text $t (unit.o)\n'
                            '  02000020 00000080 .data x (unit.o)\n'
                            '# .OVY_2\n'
                            '  02000000 00000020 .text f (unit.o)\n')
            result = parse_map(path, root, 'ARM9')
            self.assertEqual(sum(o['bytes'] for o in result), 64)
            self.assertEqual([o['status'] for o in result], ['c', 'c'])
            self.assertEqual(result[0]['source'], 'src/unit.c')

    def test_inline_and_conditional_assembly_are_not_c_only(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            path = root / 'unit.c'
            path.write_text('// asm comment\nvoid f(void) {}')
            self.assertEqual(classify(root, {'unit.c'})[0], 'c')
            path.write_text('#ifdef NONMATCHING\nvoid f(void) {}\n#else\nasm void f(void) {}\n#endif')
            self.assertEqual(classify(root, {'unit.c'})[0], 'mixed')
            self.assertEqual(classify(root, {'unit.c', 'other.c'})[0], 'unknown')

    def test_treemap_area_proportional_and_svg_escapes_names(self):
        items = [dict(bytes=n) for n in [10, 25, 1, 64]]
        rects = list(layout(items, 0, 0, 100, 50))
        for item, x, y, w, h in rects:
            self.assertAlmostEqual(w*h, item['bytes']*50)
            self.assertGreaterEqual(x, 0)
            self.assertLessEqual(x+w, 100)
            self.assertLessEqual(y+h, 50)
        report = dict(game='heartgold.us', commit='abc123', totals={'c': 10}, objects=[
            dict(cpu='ARM9', module='a&b', object='x.o', bytes=10, status='c', source='src/a&b.c')])
        ET.fromstring(svg(report))


class NamedTextTests(unittest.TestCase):
    def test_named_code_families_exclude_lookalikes_and_data(self):
        for family in CODE:
            self.assertEqual(code_family(family), family)
            self.assertEqual(code_family(family + '.routine'), family)
            self.assertIsNone(code_family(family + 'ual'))
        for section in ('.data.named', '.rodata.named', '.bss.named'):
            self.assertIsNone(code_family(section))

    def test_named_itcm_reset_functions_and_aliases(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'src').mkdir()
            (root / 'src/unit.c').write_text('void f(void) {}')
            (root / 'main.lsf').write_text('Object src/unit.o\n')
            path = root / 'test.xMAP'
            rows = ('# .ITCM\n'
                    '  01FF81B4 00000034 .itcm OSi_DoResetSystem (unit.o)\n'
                    '  01FF82C8 000000A8 .itcm OSi_ReloadRomData (unit.o)\n'
                    '  01FF8370 000000D8 .itcm OSi_ReadCardRom32 (unit.o)\n')
            path.write_text(rows)
            before = parse_map(path, root, 'ARM9')
            path.write_text(rows.replace('.itcm ', '.itcm.reset_C ') +
                            '  01FF82C8 00000180 .itcm.reset_C owner (unit.o)\n')
            after = parse_map(path, root, 'ARM9')
            self.assertEqual(before, after)
            self.assertEqual(after[0]['bytes'], 436)
            self.assertEqual(after[0]['status'], 'c')

    def test_section_rename_and_split_preserves_language_denominator(self):
        fixtures = [('c', 'void f(void) {}'),
                    ('mixed', 'asm void f(void) {}'),
                    ('asm', ''), ('unknown', None)]
        for language, contents in fixtures:
            with self.subTest(language=language), tempfile.TemporaryDirectory() as folder:
                root = Path(folder)
                source_dir = 'asm' if language == 'asm' else 'src'
                extension = 's' if language == 'asm' else 'c'
                (root / source_dir).mkdir()
                if contents is not None:
                    (root / source_dir / f'unit.{extension}').write_text(contents)
                (root / 'main.lsf').write_text(f'Object {source_dir}/unit.o\n')
                path = root / 'test.xMAP'
                ordinary = ('# .main\n'
                            '  02000000 00000020 .text f (unit.o)\n'
                            '  02000020 00000010 .text g (unit.o)\n')
                path.write_text(ordinary)
                before = parse_map(path, root, 'ARM9')
                named = ordinary.replace('.text f', '.text.any_prefix f').replace(
                    '.text g', '.text.any_suffix g')
                path.write_text(named +
                                '  02000000 00000020 .text.any_prefix alias (unit.o)\n'
                                '  02000000 00000030 .text owner (unit.o)\n'
                                '  02000030 00000080 .rodata.named values (unit.o)\n'
                                '  020000B0 00000080 .data.named writable (unit.o)\n'
                                '  02000130 00000080 .bss.named zeroes (unit.o)\n'
                                '  020001B0 00000080 .textual lookalike (unit.o)\n')
                after = parse_map(path, root, 'ARM9')
                self.assertEqual(before, after)
                self.assertEqual(after[0]['bytes'], 48)
                self.assertEqual(after[0]['status'], language)

    def test_named_text_overlay_addresses_stay_separate(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'src').mkdir()
            (root / 'src/unit.c').write_text('void f(void) {}')
            (root / 'main.lsf').write_text('Object src/unit.o\n')
            path = root / 'test.xMAP'
            path.write_text('# .OVY_1\n'
                            '  02200000 00000020 .text.first f (unit.o)\n'
                            '# .OVY_2\n'
                            '  02200000 00000020 .text.second g (unit.o)\n')
            self.assertEqual(sum(row['bytes'] for row in parse_map(path, root, 'ARM9')), 64)

    def test_arm7_named_text_uses_the_same_family(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'sub/lib').mkdir(parents=True)
            (root / 'sub/lib/unit.c').write_text('void f(void) {}')
            (root / 'sub/ichneumon_sub.lsf').write_text('Object lib/unit.o\n')
            path = root / 'test.xMAP'
            path.write_text('# .WRAM\n'
                            '  03800000 00000020 .text.driver f (unit.o)\n')
            result = parse_map(path, root, 'ARM7')
            self.assertEqual(result[0]['bytes'], 32)
            self.assertEqual(result[0]['status'], 'c')
            self.assertEqual(result[0]['source'], 'sub/lib/unit.c')


class DerivedObjectTests(unittest.TestCase):
    def test_archive_member_resolves_to_its_library_source(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'lib/syscall/asm').mkdir(parents=True)
            (root / 'lib/syscall/asm/_secure_IPKE.s').write_text('')
            (root / 'lib/syscall/asm/_secure_IPGE.s').write_text('')
            (root / 'main.lsf').write_text('')
            path = root / 'test.xMAP'
            path.write_text('# .main\n'
                            '  02000000 00000800 .text   .text\t(libsyscall.a _secure_ipke.o )\n'
                            '  0200008A 00000006 .text   SVC_DivRem\t(libsyscall.a _secure_ipke.o )\n')
            row = parse_map(path, root, 'ARM9')[0]
            self.assertEqual((row['status'], row['source'], row['bytes']),
                             ('asm', 'lib/syscall/asm/_secure_IPKE.s', 0x800))
            (root / 'lib/syscall/src').mkdir()
            (root / 'lib/syscall/src/_secure_IPKE.c').write_text('asm void _secure(void) {}')
            row = parse_map(path, root, 'ARM9')[0]
            self.assertEqual(row['status'], 'unknown')
            (root / 'lib/syscall/asm/_secure_IPKE.s').unlink()
            row = parse_map(path, root, 'ARM9')[0]
            self.assertEqual((row['status'], row['source']), ('mixed', 'lib/syscall/src/_secure_IPKE.c'))

    def test_partitioned_object_keeps_its_producer_source(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'src').mkdir()
            (root / 'src/unit.c').write_text('void f(void) {}\nasm void g(void) {}')
            (root / 'common.mk').write_text(
                '$(BUILD_DIR)/asm/unit_part.o: $(BUILD_DIR)/src/unit.o tools/partition.py\n')
            (root / 'main.lsf').write_text('Object asm/unit_part.o\n')
            path = root / 'test.xMAP'
            path.write_text('# .main\n  02000000 00000020 .text g (unit_part.o)\n')
            row = parse_map(path, root, 'ARM9')[0]
            self.assertEqual((row['status'], row['source']), ('mixed', 'src/unit.c'))
            (root / 'common.mk').write_text('')
            self.assertEqual(parse_map(path, root, 'ARM9')[0]['status'], 'unknown')

    def test_dsprot_encoded_decrypter_is_generated_assembly(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'lib/dsprot/src').mkdir(parents=True)
            (root / 'lib/dsprot/build').mkdir()
            (root / 'lib/dsprot/src/unit.c').write_text('void f(void) {}')
            (root / 'lib/dsprot/build/unit_decrypter.s').write_text('')
            (root / 'main.lsf').write_text('Object lib/dsprot/unit_encrypted.o\n'
                                           'Object lib/dsprot/unit_decrypter_encoded.o\n')
            path = root / 'test.xMAP'
            path.write_text('# .main\n'
                            '  02000000 00000020 .text f (unit_encrypted.o)\n'
                            '  02000020 00000010 .text g (unit_decrypter_encoded.o)\n')
            rows = {r['object']: (r['status'], r['source']) for r in parse_map(path, root, 'ARM9')}
            self.assertEqual(rows['unit_encrypted.o'], ('c', 'lib/dsprot/src/unit.c'))
            self.assertEqual(rows['unit_decrypter_encoded.o'], ('generated', 'lib/dsprot/build/unit_decrypter.s'))

    def test_dsprot_stub_sources_are_asm_functions_in_c(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'lib/dsprot/src').mkdir(parents=True)
            (root / 'lib/dsprot/build').mkdir()
            (root / 'lib/dsprot/src/unit.c').write_text('void f(void) {}')
            (root / 'lib/dsprot/src/unit_decrypter.c').write_text('asm void g(void) { bx lr }')
            (root / 'lib/dsprot/src/unit_decoder.c').write_text('static asm void h(void) { bx lr }')
            (root / 'lib/dsprot/build/unit_decrypter.ref.s').write_text('')
            (root / 'lib/dsprot/build/unit_decoder.ref.s').write_text('')
            (root / 'main.lsf').write_text('Object lib/dsprot/unit_encrypted.o\n'
                                           'Object lib/dsprot/unit_decrypter_encoded.o\n'
                                           'Object lib/dsprot/unit_decoder.o\n')
            path = root / 'test.xMAP'
            path.write_text('# .main\n'
                            '  02000000 00000020 .text f (unit_encrypted.o)\n'
                            '  02000020 00000010 .text g (unit_decrypter_encoded.o)\n'
                            '  02000030 00000010 .text h (unit_decoder.o)\n')
            rows = {r['object']: (r['status'], r['source']) for r in parse_map(path, root, 'ARM9')}
            self.assertEqual(rows['unit_encrypted.o'], ('c', 'lib/dsprot/src/unit.c'))
            self.assertEqual(rows['unit_decrypter_encoded.o'], ('mixed', 'lib/dsprot/src/unit_decrypter.c'))
            self.assertEqual(rows['unit_decoder.o'], ('mixed', 'lib/dsprot/src/unit_decoder.c'))


class CppCoverageTests(unittest.TestCase):
    def test_plain_cpp_is_explicit_and_assembly_cpp_is_mixed(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            path = root / 'unit.cpp'
            path.write_text('// asm comment\nclass Unit { public: void release() {} };')
            self.assertEqual(classify(root, {'unit.cpp'}), ('cpp', 'unit.cpp'))
            path.write_text('class Unit { public: void release() { __asm__("nop"); } };')
            self.assertEqual(classify(root, {'unit.cpp'}), ('mixed', 'unit.cpp'))

    def test_cpp_manifest_selects_the_actual_source_and_preserves_denominator(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'src').mkdir()
            (root / 'asm').mkdir()
            (root / 'src/unit.cpp').write_text('class Unit { public: void release() {} };')
            (root / 'asm/unit.s').write_text('')
            (root / 'main.lsf').write_text('Object asm/unit.o\n')
            mapfile = root / 'test.xMAP'
            mapfile.write_text('# .OVY_1\n'
                               '  02200000 00000030 .text release (unit.o)\n'
                               '  02200000 00000000 .text bridge (unit.o)\n'
                               '  02200030 00000040 .rodata values (unit.o)\n')
            before = parse_map(mapfile, root, 'ARM9')
            (root / 'main.lsf').write_text('Object src/unit.o\n')
            after = parse_map(mapfile, root, 'ARM9')
            self.assertEqual(before[0]['status'], 'asm')
            self.assertEqual((after[0]['status'], after[0]['source']), ('cpp', 'src/unit.cpp'))
            self.assertEqual(before[0]['bytes'], after[0]['bytes'])
            self.assertEqual(after[0]['bytes'], 48)

    def test_two_live_language_sources_with_one_basename_are_unresolved(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'src').mkdir()
            (root / 'src/unit.c').write_text('void release(void) {}')
            (root / 'src/unit.cpp').write_text('class Unit { public: void release() {} };')
            (root / 'main.lsf').write_text('Object src/unit.o\n')
            mapfile = root / 'test.xMAP'
            mapfile.write_text('# .OVY_1\n  02200000 00000030 .text release (unit.o)\n')
            row = parse_map(mapfile, root, 'ARM9')[0]
            self.assertEqual((row['status'], row['source']), ('unknown', None))

    def test_additive_cpp_coverage_preserves_c_only_bytes_and_percentage(self):
        old = source_coverage({'c': 32, 'unknown': 24, 'mixed': 8, 'asm': 40})
        new = source_coverage({'c': 32, 'cpp': 24, 'mixed': 8, 'asm': 40})
        self.assertEqual(old['denominator_bytes'], new['denominator_bytes'])
        self.assertEqual(old['c'], new['c'])
        self.assertEqual(new['denominator_bytes'], 104)
        self.assertEqual(new['cpp']['bytes'], 24)
        self.assertEqual(new['combined']['bytes'], 56)
        self.assertAlmostEqual(new['combined']['percent'], 100 * 56 / 104)
        self.assertEqual(source_coverage({'c': 32})['cpp'], {'bytes': 0, 'percent': 0})

    def test_cpp_svg_has_distinct_language_labels_and_combined_headline(self):
        report = dict(game='heartgold.us', commit='abc123', totals={'c': 20, 'cpp': 30, 'mixed': 50}, objects=[
            dict(cpu='ARM9', module='a&b', object='unit.o', bytes=30, status='cpp', source='src/unit.cpp')])
        rendered = svg(report)
        ET.fromstring(rendered)
        self.assertIn('50.00% C + C++ source', rendered)
        self.assertIn('C++-only source 30.0%', rendered)
        self.assertIn('C-only source 20.0%', rendered)
        self.assertIn('Mixed objects are excluded', rendered)

    def test_build_generated_assembly_is_its_own_status_and_not_source(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root / 'lib/dsprot/build').mkdir(parents=True)
            (root / 'lib/dsprot/build/x_decoder.s').write_text('.text\n')
            (root / 'asm').mkdir()
            (root / 'asm/y.s').write_text('.text\n')
            self.assertEqual(classify(root, {'lib/dsprot/build/x_decoder.s'})[0], 'generated')
            self.assertEqual(classify(root, {'asm/y.s'})[0], 'asm')
        cov = source_coverage({'c': 60, 'cpp': 20, 'mixed': 10, 'generated': 10})
        self.assertAlmostEqual(cov['combined']['percent'], 80.0)
        self.assertAlmostEqual(cov['with_asm_functions']['percent'], 90.0)
        self.assertAlmostEqual(cov['source_files_in_c']['percent'], 100.0)
        rendered = svg(dict(game='heartgold.us', commit='abc', totals={'c': 80, 'mixed': 10, 'generated': 10}, objects=[]))
        ET.fromstring(rendered)
        self.assertIn('80.00% C + C++ source', rendered)
        self.assertIn('100% of source in C/C++', rendered)
        self.assertIn('Build-tool generated 10.0%', rendered)

    def test_asm_from_local_header_macro_marks_mixed(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root / 'lib/x/src').mkdir(parents=True)
            (root / 'lib/x/include').mkdir(parents=True)
            (root / 'lib/x/include/m.h').write_text('#define F(n) asm void n(void) { bx lr }\n')
            (root / 'lib/x/src/a.c').write_text('#include "m.h"\nF(f)\n')
            (root / 'lib/x/src/b.c').write_text('#include "missing.h"\nint g;\n')
            self.assertEqual(classify(root, {'lib/x/src/a.c'})[0], 'mixed')
            self.assertEqual(classify(root, {'lib/x/src/b.c'})[0], 'c')


class OriginalAssemblyTests(unittest.TestCase):
    MAP = ('# .main\n'
           '  02000000 00000020 .text f (unit.o)\n'
           '  02000000 00000000 .text $a (unit.o)\n'
           '  02000020 00000010 .text g (unit.o)\n'
           '  02000030 00000008 .text h (unit.o)\n')

    def rows(self, source, entries, cpu='ARM9', path='src/unit.c'):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / path).parent.mkdir(parents=True)
            (root / path).write_text(source)
            (root / 'main.lsf').write_text(f'Object {Path(path).with_suffix(".o").as_posix()}\n')
            (root / 'test.xMAP').write_text(self.MAP)
            original = {(cpu, path): entries}
            return [(r['status'], r['bytes'], r.get('functions')) for r in parse_map(root / 'test.xMAP', root, 'ARM9', original)]

    def test_listed_asm_function_is_split_and_the_c_rest_is_c(self):
        source = 'asm void g(void);\nvoid f(void) { g(); }\nasm void g(void) { bx lr }\nvoid h(void) {}\n'
        self.assertEqual(self.rows(source, {'g': 'asm'}),
                         [('c', 0x28, None), ('original_asm', 0x10, ['g'])])
        self.assertEqual(self.rows(source, {}), [('mixed', 0x38, None)])

    def test_listed_function_without_map_bytes_does_not_clear_the_rest(self):
        source = 'void f(void) {}\nasm void g(void) { bx lr }\nasm void gone(void) { bx lr }\n'
        self.assertEqual(self.rows(source, {'g': 'asm', 'gone': 'asm'}),
                         [('mixed', 0x28, None), ('original_asm', 0x10, ['g'])])

    def test_unlisted_assembly_keeps_the_rest_mixed(self):
        source = 'void f(void) {}\nasm void g(void) { bx lr }\nasm void h(void) { bx lr }\n'
        self.assertEqual(self.rows(source, {'g': 'asm'}),
                         [('mixed', 0x28, None), ('original_asm', 0x10, ['g'])])
        self.assertEqual(self.rows(source, {'g': 'asm', 'h': 'asm'}),
                         [('c', 0x20, None), ('original_asm', 0x18, ['g', 'h'])])

    def test_entry_must_match_source_form_cpu_and_path(self):
        source = 'void f(void) {}\nvoid g(void) {}\nasm void h(void) { bx lr }\n'
        # g is C in this source: listing it as original assembly changes nothing.
        self.assertEqual(self.rows(source, {'g': 'asm'}), [('mixed', 0x38, None)])
        self.assertEqual(self.rows(source, {'h': 'inline-asm'}), [('mixed', 0x38, None)])
        self.assertEqual(self.rows(source, {'h': 'asm'}, cpu='ARM7'), [('mixed', 0x38, None)])

    def test_conditional_definitions_compiled_for_this_cpu_must_be_asm(self):
        source = ('void f(void) {}\n#ifdef SDK_ARM9\nasm void g(void) { bx lr }\n#else\n'
                  'asm void g(void) { mov pc, lr }\n#endif\nvoid h(void) {}\n')
        self.assertEqual(self.rows(source, {'g': 'asm'}), [('c', 0x28, None), ('original_asm', 0x10, ['g'])])
        # Only the branch compiled for this CPU counts: an ARM7-only C definition doesn't block ARM9.
        arm7_c = source.replace('asm void g(void) { mov pc, lr }', 'void g(void) {}')
        self.assertEqual(self.rows(arm7_c, {'g': 'asm'}), [('c', 0x28, None), ('original_asm', 0x10, ['g'])])
        # A C definition in the branch this CPU compiles still blocks the entry.
        arm9_c = source.replace('asm void g(void) { bx lr }', 'void g(void) {}')
        self.assertEqual(self.rows(arm9_c, {'g': 'asm'}), [('mixed', 0x38, None)])

    def test_inline_asm_helper_counts_only_through_listed_functions(self):
        helper = 'static inline u32 Clz(u32 x) { asm { clz x, x } return x; }\n#define SHIFT(n) (31 - Clz(n))\n'
        source = helper + 'u32 f(u32 x) { return SHIFT(x); }\nu32 g(u32 x) { return x; }\nu32 h(u32 x) { return x; }\n'
        self.assertEqual(self.rows(source, {'f': 'inline-asm'}),
                         [('c', 0x18, None), ('inline_asm', 0x20, ['f'])])
        # g still inlines the helper, so the rest is not clean C.
        source = helper + 'u32 f(u32 x) { return SHIFT(x); }\nu32 g(u32 x) { return Clz(x); }\nu32 h(u32 x) { return x; }\n'
        self.assertEqual(self.rows(source, {'f': 'inline-asm'}),
                         [('mixed', 0x18, None), ('inline_asm', 0x20, ['f'])])
        # A function without any assembly is not C with inline asm.
        self.assertEqual(self.rows(source, {'h': 'inline-asm'}), [('mixed', 0x38, None)])

    def test_veneer_entries_must_be_single_literal_jumps(self):
        source = ('void f(void) {}\nasm void g(void) {\n    ldr pc, =f\n}\n'
                  'asm void h(void) {\n    ldr ip, =f\n    bx ip\n}\n')
        self.assertEqual(self.rows(source, {'g': 'veneer', 'h': 'veneer'}),
                         [('c', 0x20, None), ('veneer', 0x18, ['g', 'h'])])
        source = 'void f(void) {}\nasm void g(void) {\n    mov r0, #0\n    ldr pc, =f\n}\nvoid h(void) {}\n'
        self.assertEqual(self.rows(source, {'g': 'veneer'}), [('mixed', 0x38, None)])
        self.assertEqual(self.rows(source, {'g': 'asm'}), [('c', 0x28, None), ('original_asm', 0x10, ['g'])])

    def test_generated_entries_may_come_from_macros(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'lib/x/src').mkdir(parents=True)
            (root / 'lib/x/include').mkdir(parents=True)
            (root / 'lib/x/include/stub.h').write_text('#define STUB(n) asm void n(void) { bx lr }\n')
            (root / 'lib/x/src/s.c').write_text('#include "stub.h"\nSTUB(f)\nSTUB(g)\nasm void h(void) { bx lr }\n')
            (root / 'main.lsf').write_text('Object lib/x/src/s.o\n')
            (root / 'test.xMAP').write_text(self.MAP.replace('unit.o', 's.o'))
            def rows(entries):
                return [(r['status'], r['bytes'], r.get('functions'))
                        for r in parse_map(root / 'test.xMAP', root, 'ARM9', {('ARM9', 'lib/x/src/s.c'): entries})]
            self.assertEqual(rows({'f': 'generated', 'g': 'generated', 'h': 'generated'}),
                             [('generated', 0x38, ['f', 'g', 'h'])])
            # Whatever is left keeps the macro's assembly, so it stays mixed.
            self.assertEqual(rows({'f': 'generated', 'h': 'generated'}),
                             [('mixed', 0x10, None), ('generated', 0x28, ['f', 'h'])])
            self.assertEqual(rows({'missing': 'generated'}), [('mixed', 0x38, None)])

    def test_demangled_cpp_symbol_matches_its_source_definition(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'src').mkdir()
            (root / 'src/unit.cpp').write_text('struct T;\nasm void Jump(T *t, char *p) { bx lr }\nvoid f(void) {}\n')
            (root / 'main.lsf').write_text('Object src/unit.o\n')
            (root / 'test.xMAP').write_text('# .main\n  02000000 00000030 .text Jump(T*, char*)\t(unit.o)\n'
                                            '  02000030 00000010 .text f()\t(unit.o)\n')
            rows = parse_map(root / 'test.xMAP', root, 'ARM9', {('ARM9', 'src/unit.cpp'): {'Jump(T*, char*)': 'asm'}})
            self.assertEqual([(r['status'], r['bytes']) for r in rows], [('cpp', 0x10), ('original_asm', 0x30)])

    def test_compiled_code_excludes_hand_written_generated_and_veneer_code(self):
        cov = source_coverage({'c': 60, 'cpp': 10, 'inline_asm': 5, 'mixed': 5, 'original_asm': 20})
        self.assertEqual(cov['denominator_bytes'], 100)
        self.assertAlmostEqual(cov['combined']['percent'], 70.0)
        self.assertAlmostEqual(cov['with_asm_functions']['percent'], 100.0)
        self.assertEqual(cov['original_asm']['bytes'], 20)
        self.assertEqual((cov['compiled_code_in_c']['bytes'], cov['compiled_code_in_c']['of']), (75, 80))
        self.assertAlmostEqual(cov['compiled_code_in_c']['percent'], 93.75)
        self.assertEqual(source_coverage({'c': 32})['compiled_code_in_c']['percent'], 100.0)
        cov = source_coverage({'c': 70, 'mixed': 5, 'original_asm': 10, 'generated': 10, 'veneer': 5})
        self.assertEqual((cov['compiled_code_in_c']['bytes'], cov['compiled_code_in_c']['of']), (70, 75))
        self.assertEqual((cov['source_files_in_c']['bytes'], cov['source_files_in_c']['of']), (90, 90))
        self.assertEqual((cov['veneer']['bytes'], cov['generated']['bytes']), (5, 10))

    def test_svg_headline_legend_and_hover_for_original_assembly(self):
        objects = [dict(cpu='ARM9', module='main', object='os.o', bytes=20, status='original_asm',
                        source='src/os.c', functions=['OS_A', 'OS_B'])]
        rendered = svg(dict(game='heartgold.us', commit='abc', objects=objects,
                            totals={'c': 60, 'cpp': 10, 'inline_asm': 5, 'mixed': 5, 'original_asm': 20}))
        ET.fromstring(rendered)
        self.assertIn('>100% of source in C/C++', rendered)
        self.assertIn('93.75% compiled code in C/C++ · 70.00% C + C++ source', rendered)
        self.assertIn('Original hand-written assembly 20.0%', rendered)
        self.assertIn('C with original inline asm 5.0%', rendered)
        self.assertIn('OS_A, OS_B', rendered)
        done = svg(dict(game='heartgold.us', commit='abc', objects=[], totals={'c': 80, 'original_asm': 20}))
        self.assertIn('>100% compiled code in C/C++', done)
        tiny = svg(dict(game='heartgold.us', commit='abc', objects=[], totals={'c': 9999, 'inline_asm': 1}))
        self.assertIn('C with original inline asm &lt;0.1%', tiny)
        nearly = svg(dict(game='heartgold.us', commit='abc', objects=[], totals={'c': 999999, 'mixed': 1}))
        self.assertIn('>99.99% compiled code in C/C++', nearly)
        self.assertIn('>100% of source in C/C++', nearly)
        partial = svg(dict(game='heartgold.us', commit='abc', objects=[], totals={'c': 999998, 'mixed': 1, 'asm': 1}))
        self.assertIn('>99.99% of source in C/C++', partial)

    def test_legend_lists_non_zero_categories_and_wraps(self):
        totals = {'c': 50, 'cpp': 10, 'inline_asm': 5, 'original_asm': 10, 'generated': 10, 'veneer': 5, 'mixed': 10}
        rendered = svg(dict(game='heartgold.us', commit='abc', objects=[], totals=totals))
        ET.fromstring(rendered)
        self.assertIn('Linker-style veneers 5.0%', rendered)
        self.assertIn('Build-tool generated 10.0%', rendered)
        self.assertNotIn('>Assembly ', rendered)
        self.assertNotIn('Unresolved source', rendered)
        self.assertIn('y="113" width="12"', rendered)  # second legend row
        for x, text in re.findall(r'<text x="(\d+)" y="1(?:02|24)" font-size="14">([^<]*)<', rendered):
            self.assertLess(int(x) + len(text) * 7.7, 1172)
        single = svg(dict(game='heartgold.us', commit='abc', objects=[], totals={'c': 90, 'mixed': 10}))
        self.assertNotIn('y="113" width="12"', single)

    def test_tracked_original_assembly_list_is_valid(self):
        data = json.loads(ORIGINAL_ASM.read_text())
        entries = load_original_asm()
        self.assertEqual(sum(len(v) for v in entries.values()), len(data['functions']))
        for entry in data['functions']:
            self.assertTrue(entry['reason'].strip())
            self.assertIn(entry.get('kind', 'asm'), ('asm', 'inline-asm', 'generated', 'veneer'))
        # Compiler output that is not yet matched is unfinished work, not original assembly.
        names = {entry['function'] for entry in data['functions']}
        self.assertFalse(names & {'ov00_021FD094', 'ov13_02223EE0'})

    def test_invalid_original_assembly_list_is_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / 'original_asm.json'
            entry = dict(function='f', cpu='ARM9', source='src/a.c', reason='swp')
            path.write_text(json.dumps({'functions': [entry, entry]}))
            with self.assertRaisesRegex(ValueError, 'duplicate'):
                load_original_asm(path)
            path.write_text(json.dumps({'functions': [dict(entry, reason=' ')]}))
            with self.assertRaisesRegex(ValueError, 'invalid'):
                load_original_asm(path)

    def test_original_asm_only_checks_the_branch_compiled_for_this_cpu(self):
        with tempfile.TemporaryDirectory() as d:
            src = Path(d) / 'a.c'
            src.write_text('#ifdef SDK_ARM9\nasm void f(void) {\n    bx lr\n}\n#else\nvoid f(void) {\n}\n#endif\n')
            from progress import split_original_asm
            self.assertEqual(split_original_asm(src, {'f': 'asm'}, 'ARM9')[0], {'f': 'asm'})
            self.assertEqual(split_original_asm(src, {'f': 'asm'}, 'ARM7')[0], {})


if __name__ == '__main__':
    unittest.main()
