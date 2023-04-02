package uk.ac.man.cs.comp38211.io.array;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

public class Array2DOfIntsWritable implements Writable
{

    int[][] array;

    /**
     * Default constructor.
     */
    public Array2DOfIntsWritable()
    {
        super();
    }

    /**
     * Constructor take in a two-dimensional array. Take note for consistent
     * purpose, the constructor would re-format the array to a well shaped
     * 2-dimensional array.
     * 
     * @param array
     *            input int array
     */
    public Array2DOfIntsWritable(int[][] array)
    {
        set(array);
    }

    /**
     * Constructor that takes the size of the array as an argument.
     * 
     */
    public Array2DOfIntsWritable(int row, int column)
    {
        super();
        array = new int[row][column];
        for (int i = 0; i < row; i++)
        {
            array[row] = new int[column];
        }
    }

    public void readFields(DataInput in) throws IOException
    {
        int row = in.readInt();
        int col = in.readInt();
        array = new int[row][col];
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                setValueAt(i, j, in.readInt());
            }
        }
    }

    public void write(DataOutput out) throws IOException
    {
        out.writeInt(getNumberOfRows());
        out.writeInt(getNumberOfCols());
        for (int i = 0; i < getNumberOfRows(); i++)
        {
            for (int j = 0; j < getNumberOfCols(); j++)
            {
                out.writeInt(getValueAt(i, j));
            }
        }
    }

    public int[][] getClone()
    {
        return array.clone();
    }

    public int[][] getArray()
    {
        return array;
    }

    /**
     * Returns the int value at position <i>i</i>.
     * 
     * @param row
     *            row index of the int to be returned
     * @param col
     *            column index of the int to be returned
     * 
     * @return int value at position <i>row</i> and <i>col<\i>
     */
    public int getValueAt(int row, int col)
    {
        return array[row][col];
    }

    /**
     * Constructor take in a two-dimensional array. Take note for consistent
     * purpose, the constructor would re-format the array to a well shaped
     * 2-dimensional array.
     * 
     * @param array
     *            input int array
     */
    public void set(int[][] array)
    {
        int row = array.length;
        int col = 0;
        for (int i = 0; i < row; i++)
        {
            if (array[i] != null)
            {
                col = Math.max(col, array[i].length);
            }
        }
        this.array = array;

        for (int i = 0; i < row; i++)
        {
            if (array[i] == null)
            {
                array[i] = new int[col];
            }
            else if (array[i].length < col)
            {
                int[] temp = new int[col];
                for (int j = 0; j < array[i].length; j++)
                {
                    temp[j] = array[i][j];
                }
                array[i] = temp;
            }
        }
    }

    /**
     * Sets the int at position <i>row</i> and <i>col</i> to <i>f</i>.
     * 
     * @param f
     *            int value to be set
     */
    public void setValueAt(int row, int col, int f)
    {
        array[row][col] = f;
    }

    /**
     * Returns the number of rows in the float array.
     * 
     * @return number of rows in array
     */
    public int getNumberOfRows()
    {
        return array.length;
    }

    /**
     * Returns the number of columns in the array.
     * 
     * @return number of columns in the array
     */
    public int getNumberOfCols()
    {
        return array[0].length;
    }

    public String toString()
    {
        String s = "[";
        for (int i = 0; i < getNumberOfRows(); i++)
        {
            for (int j = 0; j < getNumberOfCols() - 1; j++)
            {
                s += getValueAt(i, j) + ", ";
            }
            s += getValueAt(i, getNumberOfCols() - 1) + "; ";
        }
        s += "]";
        return s;
    }
}
