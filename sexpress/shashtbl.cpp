// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include "shashtbl.hpp"
#if INTELIB_TEXT_REPRESENTATIONS == 1
#  include "sstring.hpp"
#endif


static const intelib_hash_t hash_multiplier = 0x9c406bb5;
// Recommended by Knuth

static const intelib_hash_t hash_xor_op = 0x12fade34;
// arbitrary

static const unsigned long hash_sizes[] = {
            11,     /* > 8 */
            17,     /* > 16 */
            37,     /* > 32 */
            67,     /* > 64 */
            131,    /* > 128 */
            257,    /* > 256 */
            521,    /* > 512 */
            1031,   /* > 1024 */
            2053,   /* > 2048 */
            4099,   /* > 4096 */
            8209,   /* > 8192 */
            16411,  /* > 16384 */
            32771,  /* > 32768 */
            65537,  /* > 65536 */
            131101, /* > 131072 */
            262147, /* > 262144 */
            524309, /* > 524288 */
            1048583, /* > 1048576 */
            2097169, /* > 2097152 */
            4194319, /* > 4194304 */
            8388617, /* > 8388608 */
            16777259, /* > 16777216 */
            33554467, /* > 33554432 */
            67108879, /* > 67108864 */
            134217757, /* > 134217728 */
            /* we don't need sizes larger that 2**27; */
            /* allocating such an array would crash the program anyway */
            0       /* end-of-table mark */
        };


intelib_hash_t UniversalHash(intelib_hash_t l)
{
    return hash_multiplier * (l ^ hash_xor_op);
}

static
intelib_hash_t CharStringSum(const char *s)
{
    register intelib_hash_t res = 7;
    // Arbitrary; to enforce Hash(0) != Hash("").

    register int i = 0;
    while(*s) {
        res ^= (((intelib_hash_t) *s) << (8*(i%4)));
        // Every byte is XOR'ed with 0th, 1st, 2nd, 3rd, 0th... byte of res.
        s++; i++;
    }
    return res;
}

#if 0
static
intelib_hash_t MemAreaSum(const unsigned char *p, int l)
{
    intelib_hash_t res = 13; // Arbitrary; to enforce Hash(0) != Hash("").
    register int i;
    for(i=0; i<l; i++) {
        res ^= (((intelib_hash_t) p[i]) << (8*(i%4)));
        // Every byte is XOR'ed with 0th, 1st, 2nd, 3rd, 0th... byte of res.
    }
    return res;
}
#endif

intelib_hash_t LispHash(const SReference &ref) {
    // Keep It Simple, Stupid...
    return UniversalHash(CharStringSum(ref->TextRepresentation().c_str()));
    // This might be too slow for particular purposes so... so what? ;-)
}


bool SExprsAreEq(SReference l1, SReference l2)
{ return l1 == l2; }
bool SExprsAreEql(SReference l1, SReference l2)
{ return l1.IsEql(l2); }
bool SExprsAreEqual(SReference l1, SReference l2)
{ return l1.IsEqual(l2); }

//////////////////////////////////////////
// class SExpressionHashTable

IntelibTypeId SExpressionHashTable::TypeId(&SExpression::TypeId, true);

SLabel SExpressionHashTable::EmptySlotMark("#<HASH_EMPTY_SLOT>");

SExpressionHashTable::SExpressionHashTable(SEqualityPredicate eqp)
        : SExpression(TypeId)
{
    dim = hash_sizes[0];
    comp = eqp ? eqp : SExprsAreEqual;
    table = new SReference[dim];
    itemcount = 0;
    lastfoundpos = -1;
}

SExpressionHashTable::SExpressionHashTable(const IntelibTypeId &id,
        SEqualityPredicate eqp)
        : SExpression(id)
{
    dim = hash_sizes[0];
    comp = eqp ? eqp : SExprsAreEqual;
    table = new SReference[dim];
    itemcount = 0;
    lastfoundpos = -1;
}

SExpressionHashTable::~SExpressionHashTable()
{
    delete[] table;
}

SExpression* SExpressionHashTable::Clone() const
{
    SExpressionHashTable *ret = new SExpressionHashTable(comp);
    delete[] ret->table;
    ret->dim = dim;
    ret->table = new SReference[dim];
    ret->itemcount = itemcount;
    ret->lastfoundpos = lastfoundpos;
    for(unsigned int i=0; i<dim; i++)
        ret->table[i] = table[i].Clone();
    return ret;
}

void SExpressionHashTable::AddItem(SReference key, SReference val)
{
    GetItemPosition(key) = val;
}

bool SExpressionHashTable::SafeAddItem(SReference key, SReference val)
{
    SReference &pos = GetItemPosition(key);
    if(pos.GetPtr() != EmptySlotMark.GetPtr())
        return false;
    else {
        pos = val;
        return true;
    }
}

SReference SExpressionHashTable::
FindItem(SReference key, const SReference &defval) const
{
    unsigned long pos = LispHash(key) % dim;
    while(table[pos].GetPtr()) {
        SExpressionCons *dp =
            static_cast<SExpressionCons*>(table[pos].GetPtr());
        if((comp)(dp->Car(), key)) {
            // found !
            return dp->Cdr();
        }
        pos = pos ? pos-1 : dim-1;
    }
    // Not found
    return SReference(defval);
}

SReference& SExpressionHashTable::GetItemPosition(SReference key)
{
    HandleLastFoundPos();
    intelib_hash_t pos = LispHash(key) % dim;
    while(table[pos].GetPtr()) {
        SExpressionCons *dp =
            static_cast<SExpressionCons*>(table[pos].GetPtr());
        if((comp)(dp->Car(), key)) {
            // found !
            return dp->Cdr();
        }
        pos = pos ? pos-1 : dim-1;
    }
    itemcount++;
    if(itemcount * 3 > dim * 2) {
        // This is rare event so we can leave it unoptimized
        ResizeTable();
        // table changed; recompute the pos assuming the item isn't found
        pos = LispHash(key) % dim;
        while(table[pos].GetPtr()) pos = pos ? pos-1 : dim-1;
        // ResizeTable destroyed our itemcount
        itemcount++;
    }
    SExpressionCons *cdp;
    table[pos] = SReference(cdp = new SExpressionCons(key, EmptySlotMark));
    lastfoundpos = pos;
    return cdp->Cdr();
}

bool SExpressionHashTable::RemoveItem(SReference key)
{
    intelib_hash_t pos = LispHash(key) % dim;
    while(table[pos].GetPtr()) {
        SExpressionCons *dp =
            static_cast<SExpressionCons*>(table[pos].GetPtr());
        if((comp)(dp->Car(), key)) {
            DoRemoveItem(pos);
        }
        pos = pos ? pos-1 : dim-1;
    }
    // Not found
    return false;
}

void SExpressionHashTable::Clear()
{
    for(unsigned int i = 0; i< dim; i++)
        table[i] = SReference();
    itemcount = 0;
    lastfoundpos = -1;
}

SString SExpressionHashTable::TextRepresentation() const
{
    SString res("#S(HASH-TABLE ");
    if(comp == SExprsAreEq)
        res += "EQ";
    else
    if(comp == SExprsAreEql)
        res += "EQL";
    else
    if(comp == SExprsAreEqual)
        res += "EQUAL";
    else
        res += "???";
    HandleLastFoundPos();
    for(unsigned int i = 0; i< dim; i++) {
        if(table[i].GetPtr()) {
            res += " ";
            res += table[i]->TextRepresentation();
        }
    }
    res += ")";
    return res;
}

void SExpressionHashTable::DoRemoveItem(long pos)
{
    /* We implement the Don Knuth' algorythm of removing items */
    /* see vol. 3, section 6.4 */
    itemcount--;
    table[pos] = SReference();
    for(long i = pos?pos-1:dim-1; table[i].GetPtr(); i =i?i-1:dim-1) {
        SExpressionCons *dp = static_cast<SExpressionCons*>(table[i].GetPtr());
        long r = LispHash(dp->Car()) % dim;
        if( (i <= r && r < pos) ||
                ((pos < i) && (r < pos || i <= r))
          ) continue;
        table[pos] = table[i];
        table[i] = SReference();
        pos = i;
    }
}

void SExpressionHashTable::ResizeTable()
{
    HandleLastFoundPos();
    unsigned long newdim = 0;
    for(int i=1; hash_sizes[i]; i++) {
        if(hash_sizes[i] > dim) {
            newdim = hash_sizes[i];
            break;
        }
    }
    if(!newdim) throw IntelibX_hash_table_too_big(SReference(this));
    long olddim = dim;
    SReference *oldtable = table;
    dim = newdim;
    table = new SReference[newdim];
    itemcount = 0;
    for(long i = 0; i<olddim; i++) {
        if(oldtable[i].GetPtr()) {
            SExpressionCons *dp =
                static_cast<SExpressionCons*>(oldtable[i].GetPtr());
            GetItemPosition(dp->Car()) = dp->Cdr();
            lastfoundpos = -1; // for efficiency
        }
    }
    delete [] oldtable;
}

void SExpressionHashTable::HandleLastFoundPos() const
{

    SExpressionHashTable *p = const_cast<SExpressionHashTable*>(this);
    if(lastfoundpos != -1) {
        SExpressionCons *dp =
            static_cast<SExpressionCons*>(table[lastfoundpos].GetPtr());
        if(dp && dp->Cdr().GetPtr() == EmptySlotMark.GetPtr()) {
            table[lastfoundpos] = SReference();
            p->itemcount--;
        }
        p->lastfoundpos = -1;
    }
}

SExpressionHashTable::Iterator::Iterator(const SExpressionHashTable &ref)
{
    ref.HandleLastFoundPos();
    tbl = ref.table;
    idx = 0;
    lim = ref.dim;
}


SReference& SExpressionHashTable::Iterator::GetNext()
{
#if 1
    while(idx<lim &&
            (!tbl[idx].GetPtr() ||
             (static_cast<SExpressionCons*>(tbl[idx].GetPtr())->Cdr().GetPtr()
              == EmptySlotMark.GetPtr())
            )
         )
#else 
    while(idx<lim && !tbl[idx].GetPtr())
#endif
    {
        idx++;
    }
    if(idx>=lim) {
        static SReference unbound;
        return unbound;
    }
    return tbl[idx++];
}


///////////////////////////////////////////////////////////

IntelibX_not_a_hash_table::
IntelibX_not_a_hash_table(SReference a_param)
        : IntelibX("Not a hash table", a_param) {}

IntelibX_hash_table_too_big::
IntelibX_hash_table_too_big(SReference a_param)
        : IntelibX("Hash table too big", a_param) {}

