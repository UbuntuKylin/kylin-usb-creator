#include "stylewidgetattribute.h"

StyleWidgetAttribute::StyleWidgetAttribute()
{

}

StyleWidgetAttribute::StyleWidgetAttribute(int style_w, int style_h, bool style_allRadius, int style_radius,int style_shadow,
                                           double style_shadowAlpha, int style_titleHeight, int style_itemHeight, bool style_middle)
{
    w=style_w+style_shadow*2;
    h=style_h+style_shadow*2;
    allRadius=style_allRadius;
    radius=style_radius;
    shadow=style_shadow;
    shadowAlpha=style_shadowAlpha;
    titleHeight=style_titleHeight;
    itemHeight=style_itemHeight;
    middle=style_middle;
}

void StyleWidgetAttribute::setW(int width)
{
    w=width+shadow;
}
void StyleWidgetAttribute::setH(int height)
{
    h=height+shadow;
}
