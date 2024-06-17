#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Array2D.cpp"


using namespace std;


class DataFrame {
public:

    DataFrame(string const &fileName) {
        path_ = fileName;
        data_ = Array2D(load_data(fileName));
        row = data_.h();
        col = data_.w();
    }

    void print() {
        for (int i = 0; i < data_.row_; i++) {
            for (int j = 0; j < data_.col_; j++) {
                cout << data_.at(i, j) << " ";
            }
            cout << endl;
        }
    }

    vector<float> getColumn(const int index) {
        vector<float> values(row);
        for (int i = 0; i < row; ++i) {
            values[i] = data_.at(i, index);
        }
        return values;
    }

    vector<float> getUserIds() {
        vector<float> ids = getColumn(0);
        float max = *max_element(ids.begin(), ids.end());
        vector<float> out = vector<float>((int) max + 1);
        for (int i = 0; i < (int) max + 1; ++i) {
            out[i] = (float) i;
        }
        return out;
    }

    vector<float> getItemIds() {
        vector<float> ids = getColumn(1);
        float max = *max_element(ids.begin(), ids.end());
        vector<float> out = vector<float>((int) max + 1);
        for (int i = 0; i < (int) max + 1; ++i) {
            out[i] = (float) i;
        }
        return out;
    }

    Array2D getDataTable() {
        vector<float> userIds = getUserIds();
        vector<float> itemIds = getItemIds();
        float userCount = *max_element(userIds.begin(), userIds.end());
        float itemCount = *max_element(itemIds.begin(), itemIds.end());
        //cout << "User Count: " << userCount << " | " << "Item Count: " << itemCount << endl;

        // check counts may have a bug
        vector<vector<float> > table(userCount + 1, vector<float>(itemCount + 1));

        cout << "Table: (" << table.size() << "," << table[0].size() << ")" << endl;

        for (int i = 0; i < data_.row_; ++i) {
            //float *p = &v[0];
            auto _user_id = data_.at(i, 0);
            auto _item_id = data_.at(i, 1);
            auto _rank = data_.at(i, 2);
            //cout << _user_id << " " << _item_id << " " << _rank << endl;

            table[(unsigned long) _user_id][(unsigned long) _item_id] = _rank;
        }

        return Array2D(table);
    }


    Array2D data_;
    int row;
    int col;
private:
    string path_;

    Array2D load_data(const string &path) {
        vector<vector<string>> content;
        vector<string> row;
        string line, word;

        fstream file(path, ios::in);
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
        cout << "Data loaded from " << path << endl;
        Array2D temp = Array2D(data);
        //for (int i = 0; i < temp.row_; ++i) {
        //    for (int j = 0; j < temp.col_; ++j) {
        //        cout << temp.at(i, j) << " ";
        //    }
        //    cout << endl;
//
        //}
        return {data};
    }

};