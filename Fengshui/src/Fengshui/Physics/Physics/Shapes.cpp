//
//  Shapes.cpp
//
#include "fspch.h"

#include "Shapes.h"
#include <glm/gtc/quaternion.hpp>

static const float w = 50;
static const float h = 25;

glm::vec3 g_boxGround[] = {
	glm::vec3(-w,-h, 0 ),
	glm::vec3( w,-h, 0 ),
	glm::vec3(-w, h, 0 ),
	glm::vec3( w, h, 0 ),

	glm::vec3(-w,-h,-1 ),
	glm::vec3( w,-h,-1 ),
	glm::vec3(-w, h,-1 ),
	glm::vec3( w, h,-1 ),
};

glm::vec3 g_boxWall0[] = {
	glm::vec3(-1,-h, 0 ),
	glm::vec3( 1,-h, 0 ),
	glm::vec3(-1, h, 0 ),
	glm::vec3( 1, h, 0 ),

	glm::vec3(-1,-h, 5 ),
	glm::vec3( 1,-h, 5 ),
	glm::vec3(-1, h, 5 ),
	glm::vec3( 1, h, 5 ),
};

glm::vec3 g_boxWall1[] = {
	glm::vec3(-w,-1, 0 ),
	glm::vec3( w,-1, 0 ),
	glm::vec3(-w, 1, 0 ),
	glm::vec3( w, 1, 0 ),

	glm::vec3(-w,-1, 5 ),
	glm::vec3( w,-1, 5 ),
	glm::vec3(-w, 1, 5 ),
	glm::vec3( w, 1, 5 ),
};

glm::vec3 g_boxUnit[] = {
	glm::vec3(-0.5,-0.5,-0.5),
	glm::vec3(0.5,-0.5,-0.5),
	glm::vec3(-0.5, 0.5,-0.5),
	glm::vec3(0.5, 0.5,-0.5),

	glm::vec3(-0.5,-0.5, 0.5),
	glm::vec3(0.5,-0.5, 0.5),
	glm::vec3(-0.5, 0.5, 0.5),
	glm::vec3(0.5, 0.5, 0.5),
};

static const float t = 0.25f;
glm::vec3 g_boxSmall[] = {
	glm::vec3(-t,-t,-t ),
	glm::vec3( t,-t,-t ),
	glm::vec3(-t, t,-t ),
	glm::vec3( t, t,-t ),

	glm::vec3(-t,-t, t ),
	glm::vec3( t,-t, t ),
	glm::vec3(-t, t, t ),
	glm::vec3( t, t, t ),
};

static const float l = 3.0f;
glm::vec3 g_boxBeam[] = {
	glm::vec3(-l,-t,-t ),
	glm::vec3( l,-t,-t ),
	glm::vec3(-l, t,-t ),
	glm::vec3( l, t,-t ),

	glm::vec3(-l,-t, t ),
	glm::vec3( l,-t, t ),
	glm::vec3(-l, t, t ),
	glm::vec3( l, t, t ),
};

glm::vec3 g_boxPlatform[] = {
	glm::vec3(-l,-l,-t ),
	glm::vec3( l,-l,-t ),
	glm::vec3(-l, l,-t ),
	glm::vec3( l, l,-t ),

	glm::vec3(-l,-l, t ),
	glm::vec3( l,-l, t ),
	glm::vec3(-l, l, t ),
	glm::vec3( l, l, t ),
};

static const float t2 = 0.25f;
static const float w2 = t2 * 2.0f;
static const float h3 = t2 * 4.0f;
glm::vec3 g_boxBody[] = {
	glm::vec3(-t2,-w2,-h3 ),
	glm::vec3( t2,-w2,-h3 ),
	glm::vec3(-t2, w2,-h3 ),
	glm::vec3( t2, w2,-h3 ),

	glm::vec3(-t2,-w2, h3 ),
	glm::vec3( t2,-w2, h3 ),
	glm::vec3(-t2, w2, h3 ),
	glm::vec3( t2, w2, h3 ),
};

static const float h2 = 0.25f;
glm::vec3 g_boxLimb[] = {
	glm::vec3(-h3,-h2,-h2 ),
	glm::vec3( h3,-h2,-h2 ),
	glm::vec3(-h3, h2,-h2 ),
	glm::vec3( h3, h2,-h2 ),

	glm::vec3(-h3,-h2, h2 ),
	glm::vec3( h3,-h2, h2 ),
	glm::vec3(-h3, h2, h2 ),
	glm::vec3( h3, h2, h2 ),
};

glm::vec3 g_boxHead[] = {
	glm::vec3(-h2,-h2,-h2 ),
	glm::vec3( h2,-h2,-h2 ),
	glm::vec3(-h2, h2,-h2 ),
	glm::vec3( h2, h2,-h2 ),

	glm::vec3(-h2,-h2, h2 ),
	glm::vec3( h2,-h2, h2 ),
	glm::vec3(-h2, h2, h2 ),
	glm::vec3( h2, h2, h2 ),
};

glm::vec3 g_diamond[ 7 * 8 ];
void FillDiamond() {
	glm::vec3 pts[ 4 + 4 ];
	pts[ 0 ] = glm::vec3( 0.1f, 0, -1 );
	pts[ 1 ] = glm::vec3( 1, 0, 0 );
	pts[ 2 ] = glm::vec3( 1, 0, 0.1f );
	pts[ 3 ] = glm::vec3( 0.4f, 0, 0.4f );

	const float pi = acosf( -1.0f );
	const glm::quat quatHalf( glm::vec3( 0, 0, 2.0f * pi * 0.125f * 0.5f) );
	pts[ 4 ] = glm::vec3( 0.8f, 0, 0.3f );
	pts[ 4 ] = quatHalf * pts[ 4 ];
	pts[ 5 ] = quatHalf * pts[ 1 ];
	pts[ 6 ] = quatHalf * pts[ 2 ];

	const  glm::quat quat( glm::vec3( 0, 0, 2.0f * pi * 0.125f) );
	int idx = 0;
	for ( int i = 0; i < 7; i++ ) {
		g_diamond[ idx ] = pts[ i ];
		idx++;
	}

	glm::quat quatAccumulator;
	for ( int i = 1; i < 8; i++ ) {
		quatAccumulator = quatAccumulator * quat;
		for ( int pt = 0; pt < 7; pt++ ) {
			g_diamond[ idx ] = quatAccumulator * pts[ pt ];
			idx++;
		}
	}
}