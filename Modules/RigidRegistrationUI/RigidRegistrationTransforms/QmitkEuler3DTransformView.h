/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef QmitkEuler3DTransformViewWidgetHIncluded
#define QmitkEuler3DTransformViewWidgetHIncluded

#include "ui_QmitkEuler3DTransformControls.h"
#include "MitkRigidRegistrationUIExports.h"
#include "QmitkRigidRegistrationTransformsGUIBase.h"
#include <itkImage.h>

/*!
* \brief Widget for rigid registration
*
* Displays options for rigid registration.
*/
class MITK_RIGIDREGISTRATION_UI_EXPORT QmitkEuler3DTransformView : public QmitkRigidRegistrationTransformsGUIBase
{

public:

  QmitkEuler3DTransformView( QWidget* parent = 0, Qt::WindowFlags f = 0 );
  ~QmitkEuler3DTransformView();

  virtual mitk::TransformParameters::TransformType GetTransformType();

  virtual itk::Object::Pointer GetTransform();

  virtual itk::Array<double> GetTransformParameters();

  virtual void SetTransformParameters(itk::Array<double> transformValues);

  virtual QString GetName();

  virtual void SetupUI(QWidget* parent);

  virtual itk::Array<double> GetScales();

  virtual vtkTransform* Transform(vtkMatrix4x4* vtkmatrix, vtkTransform* vtktransform, itk::Array<double> transformParams);

  virtual int GetNumberOfTransformParameters();

private:

  template < class TPixelType, unsigned int VImageDimension >
  itk::Object::Pointer GetTransform2(itk::Image<TPixelType, VImageDimension>* itkImage1);

protected:

  Ui::QmitkEuler3DTransformControls m_Controls;

  itk::Object::Pointer m_TransformObject;

  double m_CenterX;
  double m_CenterY;
  double m_CenterZ;

};

#endif
