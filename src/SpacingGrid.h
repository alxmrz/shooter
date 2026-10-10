#ifndef SPACINGGRID_H
#define SPACINGGRID_H

#include <unordered_map>
#include <vector>
#include <cmath>

// Axis-aligned box used for clearance checks.
struct SpacingBox {
    float x;
    float y;
    int width;
    int height;
};

// Hash grid that answers "can this box be placed without violating the minimum
// clearance to already inserted boxes?" while touching only nearby cells. The
// number of geometry checks therefore grows linearly with the number of boxes,
// never quadratically.
class SpacingGrid {
public:
    SpacingGrid(int cellSize, float halfGap)
        : cellSize(cellSize), halfGap(halfGap)
    {
    }

    bool canPlace(const SpacingBox& box)
    {
        int minCx = cellIndex(box.x - halfGap);
        int maxCx = cellIndex(box.x + box.width + halfGap);
        int minCy = cellIndex(box.y - halfGap);
        int maxCy = cellIndex(box.y + box.height + halfGap);

        for (int cx = minCx; cx <= maxCx; ++cx)
        {
            for (int cy = minCy; cy <= maxCy; ++cy)
            {
                auto it = cells.find(cellKey(cx, cy));
                if (it == cells.end())
                {
                    continue;
                }
                for (const SpacingBox& other : it->second)
                {
                    ++checkCount;
                    if (overlapsExpanded(box, other))
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void insert(const SpacingBox& box)
    {
        int minCx = cellIndex(box.x - halfGap);
        int maxCx = cellIndex(box.x + box.width + halfGap);
        int minCy = cellIndex(box.y - halfGap);
        int maxCy = cellIndex(box.y + box.height + halfGap);

        for (int cx = minCx; cx <= maxCx; ++cx)
        {
            for (int cy = minCy; cy <= maxCy; ++cy)
            {
                cells[cellKey(cx, cy)].push_back(box);
            }
        }
    }

    int getCheckCount() const { return checkCount; }
    void resetCheckCount() { checkCount = 0; }

private:
    bool overlapsExpanded(const SpacingBox& a, const SpacingBox& b) const
    {
        float aL = a.x - halfGap;
        float aR = a.x + a.width + halfGap;
        float aT = a.y - halfGap;
        float aB = a.y + a.height + halfGap;
        float bL = b.x - halfGap;
        float bR = b.x + b.width + halfGap;
        float bT = b.y - halfGap;
        float bB = b.y + b.height + halfGap;
        return aL < bR && bL < aR && aT < bB && bT < aB;
    }

    int cellIndex(float value) const
    {
        return static_cast<int>(std::floor(value / cellSize));
    }

    long long cellKey(int cx, int cy) const
    {
        // Collision-free for the coordinate ranges used by the level.
        return static_cast<long long>(cx) * 100000LL + static_cast<long long>(cy);
    }

    int cellSize;
    float halfGap;
    int checkCount = 0;
    std::unordered_map<long long, std::vector<SpacingBox>> cells;
};

#endif /* SPACINGGRID_H */
