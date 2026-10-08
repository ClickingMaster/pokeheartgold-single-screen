// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {s8 sign,pad;s16 exponent;u8 length,digits[32];} Dec;
void __minus_dec(Dec *,const Dec *,const Dec *);
static inline void DecimalBorrow(u8 *place);
static inline void DecimalBorrow(u8 *place) {
    u8 *borrow=place-1;
    while(*borrow==0) --borrow;
    while(borrow!=place) {
        *borrow-=1;
        ++borrow;
        *borrow+=10;
    }
}
// @fn __minus_dec
void __minus_dec(Dec *result,const Dec *first,const Dec *second) {
    int filled;int offset;int extent;u8 *digits;u8 *end;const u8 *begin;const u8 *source;const u8 *discarded;
    *result=*first;
    if(second->digits[0]==0) return;
    filled=result->length;
    extent=filled;
    if(extent<second->length) extent=second->length;
    offset=result->exponent-second->exponent;
    extent+=offset;
    if(extent>32) extent=32;
    while(filled<extent) {
        result->length++;
        result->digits[filled]=0;
        filled=result->length;
    }
    digits=result->digits;
    end=digits+extent;
    {int aligned=second->length+offset;
     if(aligned<extent) end=digits+aligned;}
    begin=second->digits;
    {int overlap=(end-digits)-offset;source=begin+overlap;}
    discarded=source;
    while(end>digits && source>begin) {
        --end;
        --source;
        if(*end<*source) DecimalBorrow(end);
        *end-=*source;
    }
    if(discarded-begin<second->length) {
        int decrement=0;
        if(*discarded<5) decrement=1;
        else if(*discarded==5) {
            const u8 *last=second->digits+second->length;
            int position=discarded-begin;
            ++discarded;
            while(discarded<last) {
                if(*discarded!=0) goto normalize;
                ++discarded;
            }
            end=digits+position+offset-1;
            if(*end&1) decrement=1;
        }
        if(decrement) {
            if(*end<1) DecimalBorrow(end);
            --*end;
        }
    }
normalize:
    {u8 *leading=digits;
    while(*leading==0) ++leading;
    if(leading>digits) {
        u8 *last;
        u8 count=leading-digits;
        result->exponent-=count;
        last=digits+result->length;
        while(leading<last) {*digits=*leading++;digits++;}
        result->length-=count;
    }}
    {u8 *start=result->digits;u8 *stop=start+result->length;
    while(stop>start) {
        --stop;
        if(*stop!=0) break;
    }
    result->length=stop-start+1;}
}
