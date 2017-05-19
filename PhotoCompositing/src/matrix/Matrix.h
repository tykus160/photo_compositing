#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>

template<typename T> class Matrix
{
private:
    std::vector<std::vector<T>> mMatrix;
    unsigned int mRows;
    unsigned int mCols;

public:
    Matrix(unsigned int columns, unsigned int rows) :
        mCols(columns), mRows(rows)
    {
        mMatrix.resize(mRows);
        for (int i = 0; i < mRows; ++i)
        {
            mMatrix[i].resize(mCols);
        }
    }

    Matrix(unsigned int columns, unsigned int rows, const T& initialVal) :
        mCols(columns), mRows(rows)
    {
        mMatrix.resize(mRows);
        for (int i = 0; i < mRows; ++i)
        {
            mMatrix[i].resize(mCols, initialVal);
        }
    }

    T& at(unsigned int row, unsigned int col)
    {
        return mMatrix.at(row).at(col);
    }

    T& operator()(unsigned int row, unsigned int col)
    {
        return at(row, col);
    }
};

#endif // !__MATRIX_H__
