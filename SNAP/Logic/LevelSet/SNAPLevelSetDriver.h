/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SNAPLevelSetDriver.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __SNAPLevelSetDriver_h_
#define __SNAPLevelSetDriver_h_

#include "SNAPCommon.h"
#include "SnakeParameters.h"
#include "SNAPLevelSetFunction.h"
#include "SNAPLevelSetStopAndGoFilter.h"

template <class TFilter> class LevelSetExtensionFilter;
class LevelSetExtensionFilterInterface;
 
namespace itk {
  template <class TInputImage, class TOutputImage> class ImageToImageFilter;
  template <class TOwner> class SimpleMemberCommand;
  template <class TOwner> class MemberCommand;
  class Command;
};

/** 
 * \class SNAPLevelSetDriverBase
 * \brief An abstract interface that allows code to be written independently of
 * the dimensionality of the level set filter. For documentation of the methods,
 * see SNAPLevelSetDriver.
 */
class SNAPLevelSetDriverBase
{
public:
    virtual ~SNAPLevelSetDriverBase() { /*To avoid compiler warning.*/ }
  /** Set snake parameters */
  virtual void SetSnakeParameters(const SnakeParameters &parms) = 0;

  /** Run the snake for a number of iterations */
  virtual void Run(unsigned int nIterations) = 0;

  /** Restart the snake */
  virtual void Restart() = 0;

  /** Clean up the snake's state */
  virtual void CleanUp() = 0;
};

/**
 * \class SNAPLevelSetDriver
 * \brief A generic interface between the SNAP application and ITK level set
 * framework.
 *
 * This interface allows the SNAP code to exist independently of the way stop-and-go
 * level set evolution is implemented in ITK.  This gives the software a bit of 
 * modularity.  As far as SNAP cares, the public methods declared in this class are
 * the only ways to control level set evolution.
 */
template <unsigned int VDimension> 
class SNAPLevelSetDriver : public SNAPLevelSetDriverBase
{
public:

  typedef SNAPLevelSetDriver Self;

  // A callback type
  typedef itk::SmartPointer<itk::Command> CommandPointer;
  typedef itk::SimpleMemberCommand<Self> SelfCommandType;
  typedef itk::SmartPointer<SelfCommandType> SelfCommandPointer;

  /** Floating point image type used internally */
  typedef itk::Image<float, VDimension> FloatImageType;
  typedef typename itk::SmartPointer<FloatImageType> FloatImagePointer;

  /** Type definition for the level set function */
  typedef SNAPLevelSetFunction<FloatImageType> LevelSetFunctionType;
  typedef typename LevelSetFunctionType::VectorImageType VectorImageType;

  /** Initialize the level set driver.  Note that the type of snake (in/out
   * or edge) is determined entirely by the speed image and by the values
   * of the parameters.  Moreover, the type of solver used is specified in
   * the parameters as well. The last parameter is the optional external 
   * advection field, that can be used instead of the default advection
   * field that is based on the image gradient */
  SNAPLevelSetDriver(FloatImageType *initialLevelSet,
                     FloatImageType *speed,
                     const SnakeParameters &parms,
                     VectorImageType *externalAdvection = NULL);

  /** Virtual destructor */
  virtual ~SNAPLevelSetDriver() {}

  /** Set snake parameters */
  void SetSnakeParameters(const SnakeParameters &parms);

  /** Run the filter */
  void Run(unsigned int nIterations);

  /** Restart the snake */
  void Restart();

  /** Get the level set function */
  irisGetMacro(LevelSetFunction,LevelSetFunctionType *);

  /** Get the current state of the snake (level set and narrow band) */
  FloatImageType *GetCurrentState();

  /** Get the number of elapsed iterations */
  unsigned int GetElapsedIterations() const;

  /** Clean up the snake's state */
  void CleanUp();
  
private:
  /** An internal class used to invert an image */
  class InvertFunctor {
  public:
    unsigned char operator()(unsigned char input) 
      { return input == 0 ? 1 : 0; }  
  };

  /** Type definition for the level set filter */
  typedef itk::FiniteDifferenceImageFilter<FloatImageType,FloatImageType> FilterType;

  /** Level set filter wrapped by this object */
  typename FilterType::Pointer m_LevelSetFilter;

  /** Level set function used by the level set filter */
  typename LevelSetFunctionType::Pointer m_LevelSetFunction;

  /** An initialization image */
  FloatImagePointer m_InitializationImage;

  /** Last accepted snake parameters */
  SnakeParameters m_Parameters;

  /** Assign the values of snake parameters to a snake function */
  void AssignParametersToPhi(const SnakeParameters &parms, bool firstTime);

  /** Internal routines */
  void DoCreateLevelSetFilter();
};

// Type definitions
typedef SNAPLevelSetDriver<3> SNAPLevelSetDriver3d;
typedef SNAPLevelSetDriver<2> SNAPLevelSetDriver2d;

#ifndef ITK_MANUAL_INSTANTIATION
#include "SNAPLevelSetDriver.txx"
#endif

#endif // __SNAPLevelSetDriver_h_
