/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkQuadrilateralCell.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkQuadrilateralCell_txx
#define _itkQuadrilateralCell_txx
#include "itkQuadrilateralCell.h"

namespace itk
{
namespace fem
{

/**
 * Standard CellInterface:
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::CellPointer
QuadrilateralCell< TCellTraits >
::MakeCopy(void)
{
  CellPointer newCell(Self::New());
  newCell->SetPointIds(this->GetPointIds());
  return newCell;
}

  
/**
 * Standard CellInterface:
 * Get the topological dimension of this cell.
 */
template <typename TCellTraits>
int
QuadrilateralCell< TCellTraits >
::GetDimension(void) const
{
  return Self::CellDimension;
}


/**
 * Standard CellInterface:
 * Get the number of points required to define the cell.
 */
template <typename TCellTraits>
int
QuadrilateralCell< TCellTraits >
::GetNumberOfPoints(void) const
{
  return Self::NumberOfPoints;
}  


/**
 * Standard CellInterface:
 * Get the number of boundary features of the given dimension.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::CellFeatureCount
QuadrilateralCell< TCellTraits >
::GetNumberOfBoundaryFeatures(int dimension) const
{
  switch (dimension)
    {
    case 0: return GetNumberOfVertices();
    case 1: return GetNumberOfEdges();
    default: return 0;
    }
}


/**
 * Standard CellInterface:
 * Get the boundary feature of the given dimension specified by the given
 * cell feature Id.
 * The Id can range from 0 to GetNumberOfBoundaryFeatures(dimension)-1.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::CellPointer
QuadrilateralCell< TCellTraits >
::GetBoundaryFeature(int dimension, CellFeatureIdentifier featureId)
{
  switch (dimension)
    {
    case 0: return CellPointer(GetVertex(featureId));
    case 1: return CellPointer(GetEdge(featureId));
    default: return CellPointer(NULL);
    }
}


/**
 * Standard CellInterface:
 * Set the point id list used by the cell.  It is assumed that the given
 * iterator can be incremented and safely de-referenced enough times to 
 * get all the point ids needed by the cell.
 */
template <typename TCellTraits>
void
QuadrilateralCell< TCellTraits >
::SetPointIds(PointIdConstIterator first)
{
  PointIdConstIterator ii(first);
  for(int i=0; i < Self::NumberOfPoints ; ++i)
    {
    m_PointIds[i] = *ii++;
    }
}


/**
 * Standard CellInterface:
 * Set the point id list used by the cell.  It is assumed that the range
 * of iterators [first, last) contains the correct number of points needed to
 * define the cell.  The position *last is NOT referenced, so it can safely
 * be one beyond the end of an array or other container.
 */
template <typename TCellTraits>
void
QuadrilateralCell< TCellTraits >
::SetPointIds(PointIdConstIterator first, PointIdConstIterator last)
{
  int localId=0;
  PointIdConstIterator ii(first);
  
  while(ii != last)
    {
    m_PointIds[localId++] = *ii++;
    }
}


/**
 * Standard CellInterface:
 * Set an individual point identifier in the cell.
 */
template <typename TCellTraits>
void
QuadrilateralCell< TCellTraits >
::SetPointId(int localId, PointIdentifier ptId)
{
  m_PointIds[localId] = ptId;
}


/**
 * Standard CellInterface:
 * Get a begin iterator to the list of point identifiers used by the cell.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::PointIdIterator
QuadrilateralCell< TCellTraits >
::PointIdsBegin(void)
{
  return &m_PointIds[0];
}


/**
 * Standard CellInterface:
 * Get a const begin iterator to the list of point identifiers used
 * by the cell.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::PointIdConstIterator
QuadrilateralCell< TCellTraits >
::PointIdsBegin(void) const
{
  return &m_PointIds[0];
}


/**
 * Standard CellInterface:
 * Get an end iterator to the list of point identifiers used by the cell.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::PointIdIterator
QuadrilateralCell< TCellTraits >
::PointIdsEnd(void)
{
  return &m_PointIds[Self::NumberOfPoints];
}


/**
 * Standard CellInterface:
 * Get a const end iterator to the list of point identifiers used
 * by the cell.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::PointIdConstIterator
QuadrilateralCell< TCellTraits >
::PointIdsEnd(void) const
{
  return &m_PointIds[Self::NumberOfPoints];
}

/**
 * Quadrilateral-specific:
 * Get the number of vertices defining the quadrilateral.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::CellFeatureCount
QuadrilateralCell< TCellTraits >
::GetNumberOfVertices(void) const
{
  return NumberOfVertices;
}

/**
 * Quadrilateral-specific:
 * Get the number of edges defined for the quadrilateral.
 */
template < typename TCellTraits>
QuadrilateralCell< TCellTraits >::CellFeatureCount
QuadrilateralCell< TCellTraits >
::GetNumberOfEdges(void) const
{
  return Self::NumberOfEdges;
}

/**
 * Quadrilateral-specific:
 * Get the vertex specified by the given cell feature Id.
 * The Id can range from 0 to GetNumberOfVertices()-1.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::VertexPointer
QuadrilateralCell< TCellTraits >
::GetVertex(CellFeatureIdentifier vertexId)
{
  VertexPointer vert(Vertex::New());
  vert->SetPointId(0, m_PointIds[vertexId]);
  
  return vert;
}

/**
 * Quadrilateral-specific:
 * Get the edge specified by the given cell feature Id.
 * The Id can range from 0 to GetNumberOfEdges()-1.
 */
template <typename TCellTraits>
QuadrilateralCell< TCellTraits >::EdgePointer
QuadrilateralCell< TCellTraits >
::GetEdge(CellFeatureIdentifier edgeId)
{
  EdgePointer edge(Edge::New());

  for(int i=0; i < Edge::NumberOfPoints; ++i)
    {
    edge->SetPointId(i, m_PointIds[ m_Edges[edgeId][i] ]);
    }
  
  return edge;
}

/**
 * The quadrilateral's topology data: Edges.
 */
template <typename TCellTraits>
const int
QuadrilateralCell< TCellTraits >
::m_Edges[4][2] = { {0,1}, {1,2}, {2,3}, {3,0} };

} // end namespace fem
} // end namespace itk

#endif
