//--------------------------------------------
//
//     Project: Angio Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------

#ifndef liFLUOROSCOPYUNITVIEWCLASS
#define liFLUOROSCOPYUNITVIEWCLASS

#include <fltkGlWindow.h>
#include <itkPoint.h>
#include <itkLightObject.h>
#include <itkPhysicalImage.h>


namespace li 
{

  /**
   * This class emulates the imaging system of 
   * the fluoroscopy unit
   */
class FluoroscopyUnitView: public fltk::GlWindow
{


public:

  /**
   *  Constructor
   */
	FluoroscopyUnitView(int x,int y,int w,int h, const char * label=0);
	
  /**
   *  Point type for 3D 
   */
  typedef itk::Point< double, 3 > PointType; 


  /**
   *  Vector type for 3D 
   */
  typedef itk::Vector< double, 3 > VectorType; 


  /**
   *  Image type for real fluoroscopy
   */
  // Azucar
  //typedef itk::PhysicalImage< unsigned char, 2 > ImageType; 
  typedef itk::PhysicalImage< unsigned short, 2 > ImageType; 


  /**
   *  draw : overload default FLTK draw method
   */
  void draw(void);


  /**
   *  Set real fluoroscopy image
   */
  void SetFluoroscopyImage( const ImageType * );



  /**
   *  Set Position of the Source
   */
  void SetSourcePosition( const PointType & position );


  /**
   *  Get Position of the Source
   */
  const PointType & GetSourcePosition( void ) const;


  /**
   *  Set Radius of the Detector
   */
  void SetDetectorRadius( double );


  /**
   *  Set Position of the Detector
   */
  void SetDetectorPosition( const PointType & position );


  /**
   *  Get Position of the Detector
   */
  const PointType & GetDetectorPosition( void ) const;

  /**
   *  Set vertical direction of the Detector
   */
  void SetDetectorVerticalDirection( const VectorType & direction );


  /**
   *  Get vertical direction of the Detector
   */
  const VectorType & GetDetectorVerticalDirection( void ) const;


  /**
   *  Set Zoom factor
   */
  void SetZoom( double zoom );


  /**
   *  Get Zoom factor
   */
  double GetZoom( void ) const;


  /**
   *  Get the Notifier, a holder for Observers
   */
  itk::LightObject::Pointer  GetNotifier(void);


  /**
   *  Get the Projection Matrix
   */
  const GLfloat * GetProjectionMatrix(void) const;
  
  /**
   *  Get the Model Matrix
   */
  const GLfloat * GetModelMatrix(void) const;
 
private:
  /**
   *  Position of the source of X-rays.
   *  it will be used as the eye position for OpenGL drawing
   */
  PointType     m_SourcePosition;

  /**
   *  Position of the Image Detector (or Film).
   *  it will be used as the eye image position for OpenGL drawing
   */
  PointType     m_DetectorPosition;

  /**
   *  Radius of the Image Detector (or Film).
   *  it will be used to compute the fieldOfView
   */
  double        m_DetectorRadius;


  /**
   *  Direction associated with the vertical from the 
   *  point of view of the detector.  It will be used 
   *  as the up image position for OpenGL drawing
   */
  VectorType    m_DetectorVerticalDirection;


  /**
   *  Digital Zoom of the image Detector
   */
  double        m_Zoom;


  /**
   *  This light object hols all the observers that
   *  correspond to commands that will redraw objects
   *  in OpenGL.
   */
  itk::LightObject::Pointer  m_Notifier;


  /**
   *  Real fluorosocpy Image 
   */
  ImageType::ConstPointer        m_Image;


  /**
   *  Temporary image for fast OpenGL drawing and 
   *  up side down correction
   */
  unsigned char * m_ImageBackground;
  int             m_ImageBackgroundWidth;
  int             m_ImageBackgroundHeight;

  /**
   *  Matrices for projection and rigid 3D transform
   *
   */
  GLfloat         m_ProjectionMatrix[16];
  GLfloat         m_ModelMatrix[16];
  

};


} // end namespace li


#endif   
