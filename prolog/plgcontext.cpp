#include "plgcontext.hpp"

PlgContext::PlgContext() : values(), top(0)
{
    frameVars[0] = *(GetEmptyList());
}

void PlgContext::Set(const PlgReference &var, const PlgReference &value)
{
    values->AddItem(var, value);
    frameVars[top] = var ^ frameVars[top];
}

PlgReference PlgContext::Get(const PlgReference &var) const
{
    return values->FindItem(var, PlgUnbound);
}

void PlgContext::ReturnTo(int pos, bool merge)
{
    INTELIB_ASSERT(pos >= 0, IntelibX_bug());
    if (!merge) {
        // destroy bindings which will be invalid after return
        for (int i = top; i > pos; --i) {
            for (SReference p = frameVars[i]; !p.IsEmptyList(); p = p.Cdr()) {
                PlgReference var = p.Car();
                values->RemoveItem(var);
            }
        }
    }

    top = pos;
}

int PlgContext::NextFrame()
{
    top++;
    frameVars[top] = *(GetEmptyList());
    return top - 1;
}
