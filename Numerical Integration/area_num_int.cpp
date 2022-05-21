#include <iostream>
#include <omp.h>
#include <cmath>
#include <ctime>
#include <list>
using namespace std;

struct Result
{
	double timestamp, area;
};

double circle(double x, double r)
{
	return sqrt((r * r) - (x * x));
}

Result getArea(double x1, double x2, double dx, int Threads)
{
	int N = (int)((x2 - x1) / dx);
	double start = clock();
	double y = 0;
    
    /*
    The reduction clause specifies a reduction-identifier and one or more items. For each
    item, a private copy is created in each implicit task or SIMD lane and is initialized
    with the initializer value of the reduction-identifier. After the end of the region, the
    original list item is updated with the values of the private copies using the combiner
    associated with the reduction-identifier.
    reduction(reduction-identifier : list); reduction-identifier : operator
    */

	#pragma omp parallel for num_threads(Threads) reduction(+: y)
	for (int i = 1; i <= N; i++) 
        y = y + circle((x1 + (i * dx)), x2);

	y = y * dx;
	 
	return {((clock() - start) / CLOCKS_PER_SEC), y * 4};
}

int main()
{
	int Threads = 4, option = 0;
	double x1, x2, dx;

	while(true)
	{
		x1 = 0;
        dx = 0.00001;
		cout << "Enter Radius : "; 
        cin >> x2;

		list<pair<int, Result>> results;
		for(int i = 0; i < Threads; i++)
		{
			Result result = getArea(x1, x2, dx, i + 1);
			pair<int, Result> s_result(i + 1, result);
			results.push_back(s_result);
		}

		cout << endl << "Results:" << endl;
		for(auto &result : results)
		{
			cout << "   Threads Used : " << result.first 
                 << ", Time Elapsed : " << result.second.timestamp 
                 << ", Area : " << result.second.area << endl;
		}
		cout << endl;
        
        cout << "Continue? (1 / 0) : ";
        cin >> option;
        cout << endl;
        if(!option)
            break;
	}

	return 0;
}
