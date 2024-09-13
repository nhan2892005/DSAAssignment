#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#include "ann/funtions.h"
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
#include "ann/dataloader.h"

int main(int argc , char** argv) {
    int nsamples = 100;
    xt::xarray <double > X = xt:: random ::randn <double >({ nsamples , 10});
    xt::xarray <double > T = xt:: random ::randn <double >({ nsamples , 5});
    TensorDataset <double , double > ds(X, T);
    DataLoader <double , double > loader (&ds, 30, true , false);
    for(auto batch: loader){
        cout << shape2str(batch.getData().shape ()) << endl;
        cout << shape2str(batch.getLabel().shape ()) << endl;
    }
    return 0;
}