import tempfile
from pathlib import Path
import unittest
import xml.etree.ElementTree as ET

from progress import CODE, classify, code_family, generate, layout, parse_map, svg, union_size, source_coverage


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


if __name__ == '__main__':
    unittest.main()
