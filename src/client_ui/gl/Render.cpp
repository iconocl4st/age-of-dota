//
// Created by thallock on 2/6/20.
//

#include "Render.h"
#include "../app/UiClientContext.h"
#include "Color.h"

#include "../app/UiClientContext.h"
#include "../../common/state/Point.h"
#include "DebugRenderingOptions.h"
#include "../../common/state/Entity.h"

#include <math.h>

namespace aod::client::graphics {

#define Z_LEVEL_GROUND 0.0
#define Z_LEVEL_UNITS 0.1
#define Z_LEVEL_QUADTREE 0.2
#define Z_PROJECTILES 0.3

namespace colors {

Color WATER{85 / 255.0f, 67 / 255.0f, 219 / 255.0f};
Color GRASS{47 / 255.0f, 119 / 255.0f, 50 / 255.0f};
Color DESERT{163 / 255.0f, 184 / 255.0f, 121 / 255.0f};
Color UNEXPLORED{0 / 255.0f, 0 / 255.0f, 0 / 255.0f};
Color NOT_VISIBLE{35 / 255.0f, 83 / 255.0f, 39 / 255.0f};
Color GRID_LINES{140 / 255.0f, 165 / 255.0f, 100 / 255.0f};
Color CANNOT_PLACE{219 / 255.0f, 124 / 255.0f, 79 / 255.0f};
Color CAN_PLACE{76 / 255.0f, 97 / 255.0f, 80 / 255.0f};
Color MAP_BOUNDARY{255 / 255.0f, 255 / 255.0f, 255 / 255.0f};
Color BACKGROUND{88 / 255.0f, 84 / 255.0f, 160 / 255.0f};
Color DEPOSIT{0 / 255.0f, 255 / 255.0f, 0 / 255.0f};
Color COLLECT{255 / 255.0f, 175 / 255.0f, 175 / 255.0f};
Color ATTACK{255 / 255.0f, 0 / 255.0f, 0 / 255.0f};
Color BUILD{0 / 255.0f, 0 / 255.0f, 255 / 255.0f};
Color GATHER_POINT{243 / 255.0f, 191 / 255.0f, 38 / 255.0f};
Color GARDEN{48 / 255.0f, 89 / 255.0f, 50 / 255.0f};
Color PLANT{123 / 255.0f, 184 / 255.0f, 126 / 255.0f};
Color FARMING{26 / 255.0f, 214 / 255.0f, 76 / 255.0f};

Color PLAYER_COLORS[]{
	{192 / 255.0f, 192 / 255.0f, 192 / 255.0f},
	{0 / 255.0f, 0 / 255.0f, 255 / 255.0f},
	{255 / 255.0f, 0 / 255.0f, 0 / 255.0f},
	{255 / 255.0f, 255 / 255.0f, 0 / 255.0f},
	{255 / 255.0f, 175 / 255.0f, 175 / 255.0f},
};
Color SELECTED{255 / 255.0f, 255 / 255.0f, 0 / 255.0f};

}

void fillQuad(const Point& min, const Point& max, const Color& color, float z) {
	glBegin(GL_POLYGON);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(min.x, min.y, z);
		glVertex3f(max.x, min.y, z);
		glVertex3f(max.x, max.y, z);
		glVertex3f(min.x, max.y, z);
	glEnd();
}

void drawQuad(const Point& min, const Point& max, const Color& color, float z) {
	glBegin(GL_LINE_LOOP);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(min.x, min.y, z);
		glVertex3f(max.x, min.y, z);
		glVertex3f(max.x, max.y, z);
		glVertex3f(min.x, max.y, z);
	glEnd();
}

void fillCircle(const Point& center, float radius, int numVertices, const Color& color, float z) {
	glBegin(GL_POLYGON);
	glColor3f(color.red, color.green, color.blue);
	for(int ii = 0; ii < numVertices; ii++) {
		float theta = 2.0f * M_PI * float(ii) / float(numVertices);
		glVertex3f(center.x + radius * cosf(theta), center.y + radius * sinf(theta), z);
	}
	glEnd();
}

void drawCircle(const Point& center, float radius, int numVertices, const Color& color, float z) {
	glBegin(GL_LINE_LOOP);
	glColor3f(color.red, color.green, color.blue);
	for(int ii = 0; ii < numVertices; ii++) {
		float theta = 2.0f * M_PI * float(ii) / float(numVertices);
		glVertex3f(center.x + radius * cosf(theta), center.y + radius * sinf(theta), z);
	}
	glEnd();
}

void drawLine(const Point& p1, const Point& p2, const Color& color, float z) {
	glBegin(GL_LINES);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(p1.x, p1.y, z);
		glVertex3f(p2.x, p2.y, z);
	glEnd();
}

void renderGround(aod::client::app::UiClientContext& context) {
	fillQuad(
		Point{},
		Point{
			(float) context.getGameState()->clientGameState.sharedState.specification->width,
			(float) context.getGameState()->clientGameState.sharedState.specification->height
		},
		colors::GRASS,
		Z_LEVEL_GROUND
	);
}

void drawHealth(const Point &p, double maxHealth, double health, float boundingBoxWidth, float boundingBoxHeight, float barHeight) {
	double healthRatio = health / maxHealth;
	Color healthColor{
		(float)((maxHealth - health) / maxHealth),
		(float)(health / maxHealth),
		0
	};
	double healthBeginY = p.y + boundingBoxHeight;
	double healthEndY = p.y + boundingBoxHeight + barHeight;

	double healthBeginX = p.x - boundingBoxWidth;
	double healthEndX = p.x + boundingBoxWidth;
	double healthIntermediateX  = healthBeginX + healthRatio * (healthEndX - healthBeginX);
	fillQuad(
		Point{healthBeginX, healthBeginY},
		Point{healthIntermediateX, healthEndY},
		healthColor,
		Z_LEVEL_UNITS
	);
	fillQuad(
		Point{healthIntermediateX, healthBeginY},
		Point{healthEndX, healthEndY},
		Color{0, 0, 0},
		Z_LEVEL_UNITS
	);
}

void renderHuman() {

}

void renderTree() {

}

void renderEntities(aod::client::app::UiClientContext& context, GameTime currentTime) {
	auto &state = context.getGameState()->clientGameState.sharedState;

	for (auto &entityId : state.entityIds) {
		Entity entity{entityId, state};
		if (!entity.isLocked) {
			continue;
		}

		Point p = entity.getLocation(currentTime).value();
		int player = entity.getPlayer().value();
		double maxHealth = entity.getMaxHealth().value();
		double health = entity.getHealth().value();

		float radius = 0.5;
		float interactionRadius = 0.1;
		float boundingBoxWidth = 0.8;
		float boundingBoxHeight = 1.2;
		float barHeight = 0.1;

		Point interactionPoint = entity.getInteractionPoint(currentTime).value();

		fillCircle(p, radius, 30, colors::PLAYER_COLORS[player], Z_LEVEL_UNITS);
		drawLine(p, interactionPoint, colors::PLAYER_COLORS[player], Z_LEVEL_UNITS);
		fillCircle(interactionPoint, interactionRadius, 20, colors::PLAYER_COLORS[player], Z_LEVEL_UNITS);
		drawCircle(p, radius, 30, Color{0, 0, 0}, Z_LEVEL_UNITS);

		drawHealth(p, maxHealth, health, boundingBoxWidth, boundingBoxHeight, barHeight);
	}
}


void renderQuadTree(app::UiClientContext &context) {
	if (!debug_render_options::drawQuadTree) {
		return;
	}

	context.getGameState()->clientGameState.sharedState.locationManager.visitLeaves(
		Point{0, 0}, context.getGameState()->clientGameState.sharedState.getBounds(),
		[](const Point& min, const Point& max) -> void {
			drawQuad(min, max, colors::BUILD, Z_LEVEL_QUADTREE);
		}
	);

}

void drawFiredWeapons(aod::client::app::UiClientContext& context, GameTime now) {
	// lock
	auto & state = context.getGameState()->clientGameState.sharedState;
	for (auto & it : state.attacks) {
		auto& projectile = it.second;

		double innerRadius = projectile.projectileSpeed * (projectile.lastRender - projectile.launchTime);
		double outerRadius = projectile.projectileSpeed * (now - projectile.launchTime);
		if (innerRadius > projectile.range) {
			continue;
		}
		if (outerRadius > projectile.range) {
			outerRadius = projectile.range;
		}

		for (int i = 0; i < projectile.numProjectiles; i++) {
			if (projectile.isProjectileSunk(i)) {
				continue;
			}

			auto theta = (float) projectile.getOrientation(i);
			glBegin(GL_LINES);
				glColor3f(1, 1, 0);
				glVertex3f(projectile.origin.x + innerRadius * cosf(theta), projectile.origin.y + innerRadius * sinf(theta), Z_PROJECTILES);
				glVertex3f(projectile.origin.x + outerRadius * cosf(theta), projectile.origin.y + outerRadius * sinf(theta), Z_PROJECTILES);
			glEnd();
		}
		projectile.lastRender = now;
	}
}

void render(aod::client::app::UiClientContext& context, QOpenGLExtraFunctions *pFunctions) {
	std::unique_lock<std::recursive_mutex> lock{context.gameMutex};

	if (!context.isInGame()) {
		return;
	}

	double now = context.getGameState()->clientGameState.sharedState.interpolateTime(
		std::chrono::high_resolution_clock::now()
	);

	renderGround(context);
	renderEntities(context, now);
	renderQuadTree(context);

	drawFiredWeapons(context, now);


//	auto &models = context.getGameState()->models;
}

}