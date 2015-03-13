//#include </usr/include/linux/time.h>
//#include </usr/include/libiberty.h>
#include <sys/time.h>
#include <stdio.h>
#include <memory.h>
#include <map>
#include <string>
#if 0
#include <hash_map.h>
#else
#include <tr1/unordered_map>
#define hash_map std::tr1::unordered_map
#endif
using namespace std;
class CTimer
{
public:
    CTimer()
    {
        memset(&tpStart, 0, sizeof(tpStart));
        memset(&tpEnd, 0, sizeof(tpEnd));
    }
    void Begin()
    {
        gettimeofday(&tpStart, NULL);
    }
    float GetElapseTime()
    {
        gettimeofday(&tpEnd, NULL);
        float timecost = 0.0f;
        timecost = tpEnd.tv_sec - tpStart.tv_sec + (float)(tpEnd.tv_usec-tpStart.tv_usec)/1000000;
        return timecost;
    }
private:
    struct timeval tpStart;
    struct timeval tpEnd;
};
struct StTest
{
    int dd;
    size_t operator()()const
    {
        return dd;
    }
    bool operator == (const StTest &lhs)const
    {
        return this->dd == lhs.dd;
    }
    size_t operator()(const StTest &lhs)const
    {
        return lhs.dd;
    }
};
const int cnt = 40000;
int main()
{
    hash_map<int, int> imap1;
    hash_map<StTest, int, StTest> StTestMap1;
    CTimer t1;
    t1.Begin();
    for (size_t i=0; i<cnt; ++i)
    {
        imap1[i] = i;
    }
    printf("cost time : %5.5f\n", t1.GetElapseTime());
    StTest st;
    t1.Begin();
    for (size_t i=0; i<cnt; ++i)
    {
        st.dd = i;
        StTestMap1[st] = i;
    }
    printf("StTestMap1 time cost : %5.5f\n",t1.GetElapseTime());
    return 0;
}
