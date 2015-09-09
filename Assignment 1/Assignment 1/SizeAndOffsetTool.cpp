#include "camera.h"
class SizeAndOffsetTool {
public:
	int scale;
	int x;
	int y;
	int z;

	SizeAndOffsetTool(int inscale, int xOffset, int yOffset, int zOffset) {
		scale = inscale;
		x = xOffset;
		y = yOffset;
		z = zOffset;

	}

	int saoX(int coord) {
		return (scale*coord) + x;
	}

	int saoY(int coord) {
		return (scale*coord) + y;
	}
	int saoZ(int coord) {
		return (scale*coord) + z;
	}

	CVector Coordinates(int xPoint, int yPoint, int zPoint) {
		return CVector(saoX(xPoint),saoY(yPoint),saoZ(zPoint));
	}
};