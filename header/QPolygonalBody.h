#pragma once


#include "QShapeBody.h"


//!	\brief QPolygonalBody est une classe de forme polygonale utilisable par QPhysicalBody.
//!	
//!	Attention, le polygone doit respecter trois critères :
//!	 - doit être défini dans le sens horaire 
//!	 - ne doit pas s'auto croiser
//!	 - doit être fermé (le dernier point égal au premier point)
//!
//!	\code
//!												
//!					x		\					
//!			+------->		|					
//!			|				\__ COUNTERCLOCKWISE
//!			|				/					
//!		y	V				|					
//!							/					
//!												
//!							\					
//!		y	^				|					
//!			|				\__ CLOCKWISE		
//!			|				/					
//!			+------->		|					
//!					x		/					
//!												
//! \endcode
//!
class QPolygonalBody : public QShapeBody
{
public:
	QPolygonalBody() = default;
	QPolygonalBody(QPolygonF const & polygon);

	virtual QShapeBody * clone() const override;

	QPolygonF polygon() const;
	void setPolygon(QPolygonF const & polygon);

	virtual void draw(QPainter & painter) const override;

protected:
	QPolygonF mPolygon;

	virtual void processInertia() override;
};

