#include "gvtests.h"


void runTestSort()
{
    //TEST SORT INDEX
    double arrTest[8] = {-28,1,6.9,3,4,5,7,-6.2};
    unsigned char indexTest[8];

    sort(arrTest, indexTest);

    cout << "Array Sort Test" << endl;
    for(int i = 0 ; i < 8; i++)
    {
        cout << (int)indexTest[i] << endl;
    }

    cout << "END TESTS" << endl;
}
