#ifndef LINE_LINE_H_
#define LINE_LINE_H_

#include <gslib.h>

// 線分クラス
class Line
{
public:
	Line() = default;
	Line(const GSvector3& start, const GSvector3& end) :
		start{ start }, end{ end } {}

public:
	// 始点
	GSvector3 start{ 0.0f, 0.0f, 0.0f };
	// 終点
	GSvector3 end{ 0.0f, 0.0f, 0.0f };
};

#endif