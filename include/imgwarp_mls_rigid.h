/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef IMGTRANS_MLS_RIGID_H
#define IMGTRANS_MLS_RIGID_H

#include "imgwarp_mls.h"
#include "opencv/cv.h"
#include <vector>
using std::vector;

using cv::Mat;
using cv::Mat_;
using cv::Point_;

//! The class for MLS Rigid transform.
/*!
 * It will try to keep the image rigid. You can set preScale if you
 * can accept uniform transform.
 */
class ImgWarp_MLS_Rigid : public ImgWarp_MLS
{
public:
    //! Whether do unify scale on the points before deformation
    bool preScale;
    
    ImgWarp_MLS_Rigid();
    void calcDelta();
};

#endif // IMGTRANS_MLS_RIGID_H
