/*
    Copyright (c) 2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



/**
 * \defgroup jkqtpopencvinterface OPenCV Interfaceing Tools
 * \ingroup jkqtplotter
 *
 * Classes and functions in this group allow JKQtPlotter to directly work with OpenCV data structures.
 */

/** \file jkqtpopencvinterface.h
  * \ingroup jkqtpopencvinterface
  */

#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpdatastore.h"
#include <opencv/cv.h>

#ifndef JKQTPOPENCVINTERFACE_H
#define JKQTPOPENCVINTERFACE_H


/** \brief add one external column to the datastore. It will be filled with the contents of vector \a data.
 *
 *   \param datastore the datastore to which the OpenCV matrix shuld be added (as column)
 *   \param mat OpenCV-marix to store here
 *   \param name name for the column
 *   \param channel to copy from \a mat
 *   \return the ID of the newly created column
 *
 *   \note You need to define the Macro JKQTPLOTTER_OPENCV_INTERFACE when compiling this lib to enabled this function.
 */
inline size_t JKQTPcopyCvMatToColumn(JKQTPdatastore* datastore, const cv::Mat& mat, const QString& name=QString(""), int channel=0);





////////////////////////////////////////////////////////////////////////////////////////
namespace JKQTPdatastore_Helper {
    template <typename TPixel>
    inline void copyDataFromMat(double* data, const cv::Mat& mat, int channel) {
        size_t r=0;
        const int channels=mat.channels();
        for (int iy=0; iy<mat.rows; iy++ ) {
            const TPixel* row=mat.ptr<TPixel>(iy);
            for (int ix=0; ix<mat.cols; ix++ ) {
                data[r]=jkqtp_todouble(*static_cast<const TPixel*>(&(row[ix*channels+channel])));
                r++;
            }
        }
    }
}

inline size_t JKQTPcopyCvMatToColumn(JKQTPdatastore* datastore, const cv::Mat& mat, const QString &name, int channel)
{
    const size_t N=static_cast<size_t>(mat.cols*mat.rows);
    double* d=static_cast<double*>(malloc(static_cast<size_t>(N)*sizeof(double)));

    if (CV_MAT_DEPTH(mat.type())==CV_64F) JKQTPdatastore_Helper::copyDataFromMat<double>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_32F) JKQTPdatastore_Helper::copyDataFromMat<float>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_32S) JKQTPdatastore_Helper::copyDataFromMat<uint32_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_16S) JKQTPdatastore_Helper::copyDataFromMat<int16_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_16U) JKQTPdatastore_Helper::copyDataFromMat<uint16_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_8S) JKQTPdatastore_Helper::copyDataFromMat<int8_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_8U) JKQTPdatastore_Helper::copyDataFromMat<uint8_t>(d, mat, channel);
    else throw std::runtime_error("datatype of cv::Mat not supported by JKQTPdatastore::copyImageToColumn()");

    size_t itemid=datastore->addInternalItem(d, N);
    return datastore->addColumnForItem(itemid, 0, name);
}



#endif // JKQTPOPENCVINTERFACE_H
