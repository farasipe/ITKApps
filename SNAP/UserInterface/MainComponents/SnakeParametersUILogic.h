/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SnakeParametersUILogic.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __SnakeParametersUILogic_h_
#define __SnakeParametersUILogic_h_

#include "SnakeParametersUI.h"
#include "SNAPCommonUI.h"
#include "SnakeParameters.h"
#include "itkSmartPointer.h"

// Forward references to application classes
class GlobalState;
class IRISApplication;
class UserInterfaceBase;
class SnakeParametersPreviewPipeline;
class SimpleFileDialogLogic;
class SystemInterface;

// ITK forward references
namespace itk {
  template<class TPixel, unsigned int VDimension> class Image;
  template<class TObject> class SimpleMemberCommand;
}

/**
 * \class SnakeParametersUILogic
 * \brief Logic for the preprocessing UI.
 */
class SnakeParametersUILogic : public SnakeParametersUI
{
public:
  SnakeParametersUILogic();
  virtual ~SnakeParametersUILogic();
  
  /** Register with the parent object (required for examples to work) */
  void Register(UserInterfaceBase *parent);

  /** Initialize the internal snake parameters with external value */
  void SetParameters(const SnakeParameters &parms);

  /** Show the window */
  void DisplayWindow();

  /** Get the internal snake parameters */
  irisGetMacro(Parameters,SnakeParameters);

  /** Find out whether the user accepted the parameters or not */
  irisGetMacro(UserAccepted,bool);

  /** Provide access to the window */
  irisGetMacro(Window,Fl_Window *);

  /** This dialog can warn the user about changing solvers */
  irisSetMacro(WarnOnSolverUpdate,bool);
  irisGetMacro(WarnOnSolverUpdate,bool);

  void OnAdvectionExponentChange(Fl_Valuator *input); 
  void OnAdvectionWeightChange(Fl_Valuator *input); 
  void OnCurvatureExponentChange(Fl_Valuator *input); 
  void OnCurvatureWeightChange(Fl_Valuator *input); 
  void OnPropagationExponentChange(Fl_Valuator *input); 
  void OnPropagationWeightChange(Fl_Valuator *input); 
  void OnHelpAction(); 
  void OnOkAction(); 
  void OnCloseAction(); 
  void OnSaveParametersAction();
  void OnLoadParametersAction();
  void OnAdvancedEquationAction();

  // Advanced page
  void OnTimeStepAutoAction();
  void OnTimeStepChange(Fl_Valuator *input);
  void OnSmoothingWeightChange(Fl_Valuator *input);
  void OnLegacyClampChange(Fl_Check_Button * input);
  void OnLegacyGroundChange(Fl_Valuator *input);
  void OnSolverChange();

  // Force example
  void OnAnimateAction();

  // This method should be called when the speed color map is updated
  void OnSpeedColorMapUpdate();

  // Help System
  void ShowHelp(const char *link);

  // Redraw the boxes
  void RedrawAllBoxes();

private:
  /** Called internally when the parameters change */
  void OnParameterUpdate();

  /** Internal parameter values */
  SnakeParameters m_Parameters;

  /** Whether or not the user accepted the parameter values */
  bool m_UserAccepted;

  // Image used to demonstrate examples
  typedef itk::Image<float,2> ExampleImageType;
  typedef itk::SmartPointer<ExampleImageType> ExampleImagePointer;

  /** Internally used example images */
  ExampleImagePointer m_ExampleImage[2];

  /** The preview pipeline used by all the preview windows */
  SnakeParametersPreviewPipeline *m_PreviewPipeline;
  
  /** Whether or not to warn if the user tries to change the solver */
  bool m_WarnOnSolverUpdate;

  /** Parent user interface, needed for invoking the help system */
  UserInterfaceBase *m_ParentUI;

  /** A pointer to the system interface object */
  SystemInterface *m_SystemInterface;

  /** An interface used to save and load parameters */
  SimpleFileDialogLogic *m_IODialog;

  // IO Dialog callback functions
  void LoadParametersCallback();
  void SaveParametersCallback();

  // FLTK timer callback for animation
  static void OnTimerCallback(void *);

  // Common code for closing the window
  void CloseWindow();
};

#endif // __SnakeParametersUILogic_h_
