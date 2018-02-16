#ifndef Q_PAINTER_ARTIST_H
#define Q_PAINTER_ARTIST_H


#include <QPainter>
#include <QPointF>


class QPainterArtist
{
public:
	QPainterArtist() = delete;
	~QPainterArtist() = delete;
	QPainterArtist(QPainterArtist const & m) = delete;
	QPainterArtist& operator=(QPainterArtist const & m) = delete;
	QPainterArtist(QPainterArtist const && m) = delete;
	QPainterArtist& operator=(QPainterArtist const && m) = delete;

	static QColor changeTransparency(QColor color, qreal transparency);
	static QBrush changeTransparency(QBrush brush, qreal transparency);
	static QPen changeTransparency(QPen pen, qreal transparency);
	static QColor randomColor(qreal minHue = 0.0, qreal maxHue = 1.0, qreal minSaturation = 0.0, qreal maxSaturation = 1.0, qreal minLightness = 0.0, qreal maxLightness = 1.0);
	
	static QBrush brush() { return sBrush; }
	static QPen pen() { return sPen; }
	static void setBrush(QBrush const & brush) { sBrush = brush; }
	static void setPen(QPen const & pen) { sPen = pen; }
	static void setBrushPen(QBrush const & brush, QPen const & pen) { sBrush = brush; sPen = pen; }

	static void drawCentroid(QPainter & painter, QPointF const & centroid, qreal radius);

protected:
	static QBrush sBrush;
	static QPen sPen;
};


#endif // Q_PAINTER_ARTIST_H
