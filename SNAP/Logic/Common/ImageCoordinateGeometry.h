/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageCoordinateGeometry.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __ImageCoordinateGeometry_h_
#define __ImageCoordinateGeometry_h_

#include "ImageCoordinateTransform.h"

/**
 * \class ImageCoordinateGeometry
 * \brief This class describes the geometric relationship between the image 
 * coordinate system, the patient coordinate system, and the display coordinate
 * system.
 */
class ImageCoordinateGeometry
  {
public:
  /** Constructor initializes geometry with default identity transforms */
  ImageCoordinateGeometry();

  /** Constructor that calls GetGeometry */
  ImageCoordinateGeometry(std::string imageAnatomyRAICode,
                          std::string displayAnatomyRAICode[3],
                          const Vector3ui &imageSize);

  /** Virtual destructor */
  virtual ~ImageCoordinateGeometry() {}

  /** Initializes geometry with tranforms specified by character
   * RAI codes for the image-anatomy and display-anatomy transforms and the 
   * image size */
  void SetGeometry(std::string imageAnatomyRAICode,
                   std::string displayAnatomyRAICode[3],
                   const Vector3ui &imageSize);

  /** Get the transform from image to patient coordinate system */
  irisGetMacro(ImageToAnatomyTransform,const ImageCoordinateTransform &);

  /** Get the transform from patient to display coordinate system */
  const ImageCoordinateTransform & GetAnatomyToDisplayTransform(unsigned int i)
  {
    return m_AnatomyToDisplayTransform[i];
  }

  /** Get the transform from image to display coordinate system */
  const ImageCoordinateTransform & GetImageToDisplayTransform(unsigned int i)
  {
    return m_ImageToDisplayTransform[i];
  }

  /** Get the transform from display to image coordinate system */  
  const ImageCoordinateTransform & GetDisplayToImageTransform(unsigned int i)
  {
    return m_DisplayToImageTransform[i];
  }

  /** Check an RAI orientation code for validity */
  static bool IsRAICodeValid(const char *code);

  /** Map an RAI code to a mapping vector of positive or negative 1,2,3 */
  static Vector3i ConvertRAIToCoordinateMapping(const char *code);  

  /** Invert a mapping vector of 1,2,3 */
  static Vector3i InvertMappingVector(const Vector3i &mapping);

private:

  // Slice transform information
  ImageCoordinateTransform m_ImageToAnatomyTransform;
  ImageCoordinateTransform m_AnatomyToDisplayTransform[3];

  // Three anatomy to display transforms (for each of the 3D slices)
  ImageCoordinateTransform m_ImageToDisplayTransform[3];
  ImageCoordinateTransform m_DisplayToImageTransform[3];
  };

#endif
