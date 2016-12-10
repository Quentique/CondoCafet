#ifndef COLOR_WHEEL_HPP
#define COLOR_WHEEL_HPP

//#include "colorwidgets_global.hpp"

#include <QWidget>
#include "qmath.h"

namespace color_widgets {
namespace detail {


inline qreal color_chromaF(const QColor& c)
{
    qreal max = qMax(c.redF(), qMax(c.greenF(), c.blueF()));
    qreal min = qMin(c.redF(), qMin(c.greenF(), c.blueF()));
    return max - min;
}

inline qreal color_lumaF(const QColor& c)
{
    return 0.30 * c.redF() + 0.59 * c.greenF() + 0.11 * c.blueF();
}
QColor color_from_lch(qreal hue, qreal chroma, qreal luma, qreal alpha = 1 );

inline QColor rainbow_lch(qreal hue)
{
    return color_from_lch(hue,1,1);
}

inline QColor rainbow_hsv(qreal hue)
{
    return QColor::fromHsvF(hue,1,1);
}

inline qreal color_lightnessF(const QColor& c)
{
    return ( qMax(c.redF(),qMax(c.greenF(),c.blueF())) +
             qMin(c.redF(),qMin(c.greenF(),c.blueF())) ) / 2;
}

inline qreal color_HSL_saturationF(const QColor& col)
{
    qreal c = color_chromaF(col);
    qreal l = color_lightnessF(col);
    if ( qFuzzyCompare(l+1,1) || qFuzzyCompare(l+1,2) )
        return 0;
    return c / (1-qAbs(2*l-1));
}

QColor color_from_hsl(qreal hue, qreal sat, qreal lig, qreal alpha = 1 );

}

class ColorWheel : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE true STORED false USER true )
    Q_PROPERTY(qreal hue READ hue WRITE setHue DESIGNABLE false )
    Q_PROPERTY(qreal saturation READ saturation WRITE setSaturation DESIGNABLE false )
    Q_PROPERTY(qreal value READ value WRITE setValue DESIGNABLE false )
    Q_PROPERTY(unsigned wheelWidth READ wheelWidth WRITE setWheelWidth DESIGNABLE true )
    Q_PROPERTY(DisplayFlags displayFlags READ displayFlags WRITE setDisplayFlags NOTIFY displayFlagsChanged DESIGNABLE true )

public:
    enum DisplayEnum
    {
        SHAPE_DEFAULT  = 0x000, ///< Use the default shape
        SHAPE_TRIANGLE = 0x001, ///< A triangle
        SHAPE_SQUARE   = 0x002, ///< A square
        SHAPE_FLAGS    = 0x00f, ///< Mask for the shape flags

        ANGLE_DEFAULT  = 0x000, ///< Use the default rotation style
        ANGLE_FIXED    = 0x010, ///< The inner part doesn't rotate
        ANGLE_ROTATING = 0x020, ///< The inner part follows the hue selector
        ANGLE_FLAGS    = 0x0f0, ///< Mask for the angle flags

        COLOR_DEFAULT  = 0x000, ///< Use the default colorspace
        COLOR_HSV      = 0x100, ///< Use the HSV color space
        COLOR_HSL      = 0x200, ///< Use the HSL color space
        COLOR_LCH      = 0x400, ///< Use Luma Chroma Hue (Y_601')
        COLOR_FLAGS    = 0xf00, ///< Mask for the color space flags

        FLAGS_DEFAULT  = 0x000, ///< Use all defaults
        FLAGS_ALL      = 0xfff  ///< Mask matching all flags
    };
    Q_DECLARE_FLAGS(DisplayFlags, DisplayEnum)
    Q_FLAGS(DisplayFlags)

    explicit ColorWheel(QWidget *parent = 0);
    ~ColorWheel();

    /// Get current color
    QColor color() const;

    QSize sizeHint() const;

    /// Get current hue in the range [0-1]
    qreal hue() const;

    /// Get current saturation in the range [0-1]
    qreal saturation() const;

    /// Get current value in the range [0-1]
    qreal value() const;

    /// Get the width in pixels of the outer wheel
    unsigned int wheelWidth() const;

    /// Set the width in pixels of the outer wheel
    void setWheelWidth(unsigned int w);

    /// Get display flags
    DisplayFlags displayFlags(DisplayFlags mask = FLAGS_ALL) const;

    /// Set the default display flags
    static void setDefaultDisplayFlags(DisplayFlags flags);

    /// Get default display flags
    static DisplayFlags defaultDisplayFlags(DisplayFlags mask = FLAGS_ALL);

    /**
     * @brief Set a specific display flag
     * @param flag  Flag replacing the mask
     * @param mask  Mask to be cleared
     */
    void setDisplayFlag(DisplayFlags flag, DisplayFlags mask);

public Q_SLOTS:

    /// Set current color
    void setColor(QColor c);

    /**
     * @param h Hue [0-1]
     */
    void setHue(qreal h);

    /**
     * @param s Saturation [0-1]
     */
    void setSaturation(qreal s);

    /**
     * @param v Value [0-1]
     */
    void setValue(qreal v);

    /**
     * @brief Set the display flags
     * @param flags which will replace the current ones
     */
    void setDisplayFlags(ColorWheel::DisplayFlags flags);

Q_SIGNALS:
    /**
     * Emitted when the user selects a color or setColor is called
     */
    void colorChanged(QColor);

    /**
     * Emitted when the user selects a color
     */
    void colorSelected(QColor);

    void displayFlagsChanged(ColorWheel::DisplayFlags flags);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

private:
    class Private;
    Private * const p;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ColorWheel::DisplayFlags)

} // namespace color_widgets

#endif // COLOR_WHEEL_HPP
