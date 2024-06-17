#include <iostream>
#include <vector>

using namespace std;

class Array2D {
public:
    Array2D() {
        row_ = 0;
        col_ = 0;
        stride_row_ = row_;
        stride_col_ = 1;
        array_ = vector<float>(row_ * col_);
    }

    Array2D(int row, int col) : row_{row}, col_{col}, stride_row_(col), stride_col_(1), array_(row * col) {
    }

    Array2D(vector<vector<float>> &array) {
        row_ = array.size();
        col_ = array[0].size();
        stride_col_ = 1;
        stride_row_ = col_;
        int c = 0;
        array_ = vector<float>(row_ * col_);
        for (int i = 0; i < array.size(); i++) {
            for (int j = 0; j < array[0].size(); j++) {
                //cout << i << "-" << j << "-" << c << endl;
                array_[c] = array[i][j];
                c++;
            }
        }
    }

    float **convert() {

        float **array2D = 0;
        array2D = new float *[row_];

        for (int h = 0; h < row_; h++) {
            array2D[h] = new float[col_];
            for (int w = 0; w < col_; w++) {
                // fill in some initial values
                // (filling in zeros would be more logic, but this is just for the example)
                array2D[h][w] = this->at(h, w);
            }
        }

        return array2D;
    }

    vector<vector<float>> convert2vector() {
        vector<vector<float>> out;
        for (int i = 0; i < row_; ++i) {
            for (int j = 0; j < col_; ++j) {
                out[i].push_back(this->at(i, j));
            }
        }
        return out;
    }

    vector<float> getRow(int rowIndex) {
        auto out = vector<float>(row_);
        for (int i = 0; i < row_; ++i) {
            out[i] = this->at(rowIndex, i);
        }
        return out;
    }

    float &at(int i, int j) {
        return array_[i * stride_row_ + j * stride_col_];
    }

    const float &at(int i, int j) const {
        return array_[i * stride_row_ + j * stride_col_];
    }

    const int h() {
        return row_;
    }

    const int w() {
        return col_;
    }

    void print() {
        for (int i = 0; i < row_; ++i) {
            for (int j = 0; j < col_; ++j) {
                cout << at(i, j) << " ";
            }
            cout << endl;
        }
    }

    Array2D transpose() {
        auto transposed = *this;
        transposed.row_ = this->col_;
        transposed.col_ = this->row_;
        transposed.stride_row_ = this->stride_col_;
        transposed.stride_col_ = this->stride_row_;
        return transposed;
    }

    int row_;
    int col_;
    int stride_row_;
    int stride_col_;
    vector<float> array_;
};