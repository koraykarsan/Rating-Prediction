#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <thread>
#include <list>
#include <algorithm>
#include <utility>
#include "DataFrame.cpp"


using namespace std;
#define THREADS_NUM 4

struct MatMult {
    int Ax, Ay, Bx, By;
    float *matA, *matB, *matC;
    int start, end;
};


void *multiply(void *ptr) {
    struct MatMult *pv = (struct MatMult *) ptr;
    int i, j, k;
    float val;

    for (i = pv->start; i < pv->end; ++i) {
        //cout << "Current: " << i << endl;
        for (j = 0; j < pv->Bx; j++) {
            val = 0;
            for (k = 0; k < pv->Ax; k++) {
                val += pv->matA[i * pv->Ax + k] * pv->matB[k * pv->Bx + j];
            }
            pv->matC[i * pv->Bx + j] = val;
        }

    }
    return nullptr;
}

int matmul(int Ax, int Ay, float *matA,
           int Bx, int By, float *matB, float *matC) {
//store thread handles
    pthread_t threads[THREADS_NUM];
//configuration structures per thread
    struct MatMult config[THREADS_NUM];
    int count_per_thread; //number of lines per thread
    int i;
// calculate number of lines per thread
    count_per_thread = Ay / THREADS_NUM;
    cout << "Thread Num: " << THREADS_NUM << endl;
    cout << "Count per Thread: " << count_per_thread << endl;
    cout << "A: " << Ay << "x" << Ax << endl;
    cout << "B: " << By << "x" << Bx << endl;

    for (i = 0; i < THREADS_NUM; i++) {
        config[i].Ax = Ax;
        config[i].Bx = Bx;
        config[i].Ay = Ay;
        config[i].By = By;
        config[i].matA = matA;
        config[i].matB = matB;
        config[i].matC = matC;
        config[i].start = i * count_per_thread;
        config[i].end = config[i].start + count_per_thread;
    }
    config[THREADS_NUM - 1].end += Ay % THREADS_NUM;
    for (i = 0; i < THREADS_NUM; i++)
        pthread_create(&threads[i], NULL, multiply, &config[i]);
    for (i = 0; i < THREADS_NUM; i++)
        pthread_join(threads[i], NULL);
    return 1;
}

vector<vector<float>> get_data(const string &fileName) {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(fileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();
            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    } else {
        cout << "Could not open the file\n" << endl;
    }
    vector<vector<float>> data;
    for (int i = 1; i < content.size(); i++) {
        vector<float> tempRow(content[i].size());
        transform(content[i].begin(), content[i].end(), tempRow.begin(), [](const string &val) {
            return stod(val);
        });
        data.push_back(tempRow);
    }
    cout << "Data loaded from " << fileName << endl;
    return data;
}

void printVector(vector<float> &array) {
    for (int i = 0; i < array.size(); ++i) {
        cout << array[i] << "\n";
    }
    cout << "Size: " << array.size() << endl;
}

template<class Type>
void printList(vector<Type> l) {
    for (auto const &i: l) {
        cout << i << endl;
    }
}


vector<int> getVoteCounts(Array2D &data) {
    //Array2D votings = Array2D((int) data.row_, 2);
    auto votings = vector<int>((int) data.row_);
    for (int user = 0; user < data.row_; ++user) {
        int count = 0;
        for (int vote = 0; vote < data.col_; ++vote) {
            if (data.at(user, vote) != (float) 0) {
                count++;
            }
        }
        votings[user] = count;
    }
    return votings;
}

vector<int> getMovieVoteCounts(Array2D &data) {
    //Array2D votings = Array2D((int) data.row_, 2);
    vector<int> votings = vector<int>((int) data.col_);
    for (int movie = 0; movie < data.col_; ++movie) {
        int count = 0;
        for (int vote = 0; vote < data.row_; ++vote) {
            if (data.at(vote, movie) != 0) {
                count++;
            }
        }
        votings[movie] = count;
    }
    return votings;
}

Array2D selectLargest(vector<int> arr, int n) {
    // create a vector of pairs to store the numbers and their indices
    vector<pair<int, int>> result;

    // store the numbers and their indices in the result vector
    for (int i = 0; i < arr.size(); i++)
        result.push_back({arr[i], i});

    // sort the result vector in descending order by the numbers
    sort(result.rbegin(), result.rend());

    // return the first n elements of the sorted vector
    auto temp = vector<pair<int, int>>(result.begin(), result.begin() + n);
    int c = 0;
    auto out_array = Array2D(10, 2);
    for (pair<int, int> p: temp) {
        out_array.at(c, 0) = p.second;
        out_array.at(c, 1) = p.first;
        c++;
    }
    return out_array;
}

Array2D selectLargest(vector<float> arr, int n) {
    // create a vector of pairs to store the numbers and their indices
    vector<pair<float, int>> result;

    // store the numbers and their indices in the result vector
    for (int i = 0; i < arr.size(); i++)
        result.push_back({arr[i], i});

    // sort the result vector in descending order by the numbers
    sort(result.rbegin(), result.rend());

    // return the first n elements of the sorted vector
    auto temp = vector<pair<float, int>>(result.begin(), result.begin() + n);
    int c = 0;
    auto out_array = Array2D(10, 2);
    for (pair<int, int> p: temp) {
        out_array.at(c, 0) = p.second;
        out_array.at(c, 1) = p.first;
        c++;
    }
    return out_array;
}

void write2col(const string fileName, Array2D &data) {
    ofstream file;
    file.open(fileName, std::ios::out);
    for (int i = 0; i < data.row_; ++i) {
        if (file.is_open()) {
            file << data.at(i, 0) << "," << data.at(i, 1) << std::endl;
        }

    }
    file.close();

}

void write(const string fileName, vector<int> &vec) {
    ofstream file;
    file.open(fileName, std::ios::out);
    for (int i = 0; i < vec.size(); ++i) {
        if (file.is_open()) {
            file << i << "," << vec[i] << std::endl;
        }

    }
    file.close();
}

void write(const string fileName, vector<float> &vec) {
    ofstream file;
    file.open(fileName, std::ios::out);
    for (int i = 0; i < vec.size(); ++i) {
        if (file.is_open()) {
            file << i << "," << vec[i] << std::endl;
        }

    }
    file.close();
}


float **getArray(unsigned int row, unsigned int col) {

    float **array2D = 0;
    array2D = new float *[row];

    for (int h = 0; h < row; h++) {
        array2D[h] = new float[col];
        for (int w = 0; w < col; w++) {
            // fill in some initial values
            // (filling in zeros would be more logic, but this is just for the example)
            array2D[h][w] = 0;
        }
    }
    return array2D;
}

Array2D calcNorms(Array2D &array) {
    auto sums = Array2D(array.row_, array.col_);
    for (int i = 0; i < array.row_; ++i) {
        for (int j = 0; j < array.col_; ++j) {
            sums.at(i, j) = sqrt((array.at(i, j) * array.at(i, j)));
        }
    }
    return sums;
}

Array2D calcSimilarities(Array2D &matmul, Array2D &norms) {
    auto sims = Array2D(matmul.row_, matmul.col_);
    for (int i = 0; i < sims.row_; ++i) {
        for (int j = 0; j < sims.col_; ++j) {
            sims.at(i, j) = matmul.at(i, j) / norms.at(i, j);
        }
    }
    return sims;
}

Array2D convertToArray(float **array, int row, int col) {
    auto res = Array2D(row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            res.at(i, j) = array[i][j];
        }
    }
    return res;
}

float predict(float userId, float itemId, Array2D &similarities, Array2D &table) {
    //find the most similar 11 user first is the user itself
    auto row = similarities.getRow(userId);

    auto top11 = selectLargest(row, 10000);

    // find their ranks
    auto out = vector<float>(10);
    for (int user = 0; user < 10; ++user) {
        out[user] = table.at(top11.at(user, 0), itemId);
    }

    float sum = 0;
    for (int i = 0; i < 10; ++i) {
        sum += out[i];
    }

    return sum / 10;


    //auto top10 = selectLargest()
}

int main() {
    string fileName = "/Users/ismailalpaydemir/Desktop/BAU/CMP2003/TermProject/train.csv";
    string out = "/Users/ismailalpaydemir/Desktop/BAU/CMP2003/TermProject/out.csv";

    DataFrame dataframe = DataFrame(fileName);

    Array2D table = dataframe.getDataTable();
    cout << "Data Table Created with Size: " << table.row_ << "x" << table.col_ << endl;

    cout << table.row_ << "-" << table.col_ << endl;
    auto voteCounts = getVoteCounts(table);
    cout << voteCounts.size() << endl;
    auto movieVoteCounts = getMovieVoteCounts(table);
    cout << movieVoteCounts.size() << endl;


    string userfile = "/Users/ismailalpaydemir/Desktop/BAU/CMP2003/TermProject/top10userTemp.csv";
    string moviefile = "/Users/ismailalpaydemir/Desktop/BAU/CMP2003/TermProject/top10movieTemp.csv";
    auto top10user = selectLargest(voteCounts, 10);
    auto top10movie = selectLargest(movieVoteCounts, 10);
    top10movie.print();
    cout << "-----------" << endl;
    top10user.print();

    write2col(userfile, top10user);
    write2col(moviefile, top10movie);
    //write(userfile, voteCounts);
    //write(moviefile, movieVoteCounts);



    Array2D tableT = table.transpose();
    cout << "Data Table Transpose Created with Size: " << tableT.row_ << "x" << tableT.col_ << endl;
    float **resultArray = getArray(table.row_, tableT.col_);
    cout << "Result Array Created with Size: " << table.row_ << "x" << tableT.col_ << endl;


    float **A = table.convert();
    float **B = tableT.convert();

    cout << "MatMul Arrays Created" << endl;

    int res = matmul(table.col_, table.row_,
                     *A, tableT.col_, tableT.row_, *B,
                     *resultArray);

    cout << "MatMul Finished. " << res << endl;
    cout << "Writing is started: ..." << endl;

    ofstream file;
    file.open(out, std::ios::out);
    for (int i = 0; i < table.row_; ++i) {
        if (file.is_open()) {
            for (int j = 0; j < tableT.col_; ++j) {
                cout << "Current: " << i << "-" << j << endl;
                file << table.at(i, j) << ",";
            }
            file << std::endl;
        }
    }
    file.close();

    auto sums = calcNorms(table);

    auto matmulResult = convertToArray(resultArray, table.row_, tableT.col_);

    auto similarities = calcSimilarities(matmulResult, sums);

    // read test file
    string testFile = "/Users/ismailalpaydemir/Desktop/BAU/CMP2003/TermProject/test.csv";

    auto testUsers = get_data(testFile);
    auto testArray = Array2D(testUsers);

    auto finalResults = Array2D(5000, 1);
    for (int i = 0; i < finalResults.row_; ++i) {
        finalResults.at(i, 0) = predict(testUsers[i][1], testUsers[i][2], similarities, table);
    }
    string finalOutFile = "/Users/ismailalpaydemir/Desktop/BAU/CMP2003/TermProject/predictions.csv";

    ofstream final;
    file.open(finalOutFile, std::ios::out);
    for (int i = 0; i < finalResults.row_; ++i) {
        if (file.is_open()) {
            file << finalResults.at(i, 0) << std::endl;
        }
    }
    file.close();
    return 0;
}

