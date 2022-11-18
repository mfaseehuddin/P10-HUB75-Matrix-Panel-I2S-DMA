#include <iostream>
using namespace std;

struct point
{
private:
    int oX;
    int oY;

public:
    int x;
    int y;
    point(int xIn, int yIn)
    {
        oX = xIn;
        oY = yIn;
        // todo: define the oY to oX Calculation Mathematically 
        oX += (oX / 8) * 8;
        // todo: can these conditions be written mathematically? -> x = oX + (oY % 4 == 0 ? 8 : 0) ✅ works
        // x = oX + (((oY >= 4 && oY <= 7) || (oY >= 11 && oY <= 14)) ? 8 : 0);
        x = oX + (oY % 4 == 0 ? 8 : 0);
        // todo: find pattern for extending this -> cant be done until I am able to chain more displays
        y = oY + ((oY >= 8 && oY <= 16) ? 8 : 0);
    }

    void debugPoint()
    {
        cout << " x: " << x << " y: " << y;
    }


};

struct Point_2
{
	int oX;
	int oY;
	int x;
	int y;
	Point_2(int x, int y)
	{
		this->oX = x;
		this->oY = y;

		// now we calculate the offset
		// System Analysis:
		// The Matrix indexing is set like this
		// one block bottom left is 0,0

		// to get a vertical alignment we need to calculate the offset
		// The Point_2s for the right most line are
		/*
		0, 0  ->  0,0
		0, 1  ->  0,1
		0, 2  ->  0,2
		0, 3  ->  0,3
		0, 4  ->  8,0
		0, 5  ->  8,1
		0, 6  ->  8,2
		0, 7  ->  8,3
		0, 8  ->  0,16
		0, 9  ->  0,17
		0, 11 ->  0,18
		0, 12 ->  0,19
		0, 13 ->  8,16
		0, 14 ->  8,17
		0, 15 ->  8,18
		0, 16 ->  8,19


		8,0 -> 8,0
		8,1 -> 8,1
		8,2 -> 8,2
		8,3 -> 8,3
		8,4 -> 16,0
		8,5 -> 16,1
		8,6 -> 16,2
		8,7 -> 16,3
		8,8 -> 8,16
		8,9 -> 8,17
		8,11 -> 8,18
		8,12 -> 8,19
		8,13 -> 16,16
		8,14 -> 16,17
		8,15 -> 16,18
		8,16 -> 16,19




		*/
		// using these Point_2s lets just draw and check
		// xmap
		/*
		0 - 0
		1 - 1
		2 - 2
		3 - 3
		4 - 4
		5 - 5
		6 - 6
		7 - 7
		8 - 16
		9 - 17
		10 - 18
		11 - 19
		12 - 20
		13 - 21
		14 - 22
		15 - 23
		16 - 32
		*/

		// calculing x offset
		if (this->oX >= 56 && this->oX <= 64)
		{
			this->oX = this->oX + 56;
		}
		else if (this->oX >= 48 && this->oX <= 55)
		{
			this->oX = this->oX + 48;
		}
		else if (this->oX >= 40 && this->oX <= 47)
		{
			this->oX = this->oX + 40;
		}
		else if (this->oX >= 32 && this->oX <= 39)
		{
			this->oX = this->oX + 32;
		}
		else if (this->oX >= 24 && this->oX <= 31)
		{
			this->oX = this->oX + 24;
		}
		else if (this->oX >= 16 && this->oX <= 23)
		{
			this->oX = this->oX + 16;
		}
		else if (this->oX >= 8 && this->oX <= 15)
		{
			this->oX = this->oX + 8;
		}

		// we can see that the offset is 8
		// so we can just add 8 to the x value
		// and we will get the correct Point_2
		// therefore
		if (oY > 3 && oY < 8 || oY > 11 && oY < 16)
		{
			this->x = oX + 8;
		}
		else
		{
			this->x = oX;
		}

		//can we represent the same mathematcially? -> yes
		// this->x = oX + (oY > 3 && oY < 8 || oY > 11 && oY < 16) * 8;
		// i.e if oY is between 4 and 7 or 12 and 15 this pattern continues
		//



		if (oY > 7 && oY < 13 || oY > 12 && oY < 17) // can this condition be simplified? -> if(oY >= 8 && oY <= 16)
		{
			this->y = oY + 8;
		}
		else
		{
			this->y = oY;
		}
	}

	void debugPoint_2()
	{
		cout << " x: " << x << " y: " << y;
	}
};



#define PANEL_X 64
#define PANEL_Y 16


int main()
{

    for (int x = 0; x < PANEL_X; x++)
    {
        for (int y = 0; y < PANEL_Y; y++)
        {
            point p(x, y);
            Point_2 p2(x, y);

            if(p.x != p2.x && p.y != p2.y)
            {
                cout << "Error at x: " << x << " y: " << y << endl;
                p.debugPoint();
                p2.debugPoint_2();
                cout << endl;
            }

        }
    }

    return 0;
}



/**
 * @result PASSING TEST!
 */