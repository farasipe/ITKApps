/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    UnaryFunctorCache.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __UnaryFunctorCache_h_
#define __UnaryFunctorCache_h_

#include <assert.h>
#include <stdio.h>

#include "SNAPCommon.h"
#include "itkMacro.h"
#include "itkProcessObject.h"

// Forward references
// template <class TInput, class TOutput, class TFunctor> class UnaryFunctorCache;
template <class TInput, class TOutput, class TFunctor> 
  class CachingUnaryFunctor;

/**
 * \class UnaryFunctorCache
 * \brief A cache for unary functors operating on types like short and char.
 *
 * This object wraps around a Functor and remembers the output values for 
 * the input values that is receives.  Do not use this class with non-integral
 * types and with types like int and long, or you will run out of memory!
 */
template <class TInput, class TOutput, class TFunctor> 
class ITK_EXPORT UnaryFunctorCache : public itk::Object
{
public:

  /** Standard class typedefs. */
  typedef UnaryFunctorCache Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Caching functor typedef
  typedef CachingUnaryFunctor<TInput,TOutput,TFunctor> CachingFunctor;

  /** New macro */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorCache,itk::Object);

  /** Evaluate the function using cache lookup */
  TOutput Evaluate(const TInput &in) const 
    {
    return m_Cache[in - m_CacheBegin];
    }

  /**
   * Set the function instance to call evaluate on
   */
  irisSetMacro(InputFunctor,TFunctor *);

  /**
   * Get the function instance
   */
  irisGetMacro(InputFunctor,TFunctor *);

  /**
   * Set the evaluation bounds, if you use these and these are small enough,
   * you can use int or long as template parameters.
   */
  void SetEvaluationRange(TInput begin, TInput end) 
  {
    m_CacheBegin = begin;

    // This code makes sure that if 'end' is the maximum value of TInput type, 
    // the length is still valid
    m_CacheLength = end;
    m_CacheLength += 1;
    m_CacheLength -= begin;
  }

  /** Compute the cache */
  void ComputeCache();

  /**
   * This method returns the lightweight functor (it can be copied)
   */
  irisGetMacro(CachingFunctor,const CachingFunctor &);

protected:

  UnaryFunctorCache();
  virtual ~UnaryFunctorCache();
  void PrintSelf(std::ostream &s, itk::Indent indent) const;

  /**
   * The function being cached
   */
  TFunctor *m_InputFunctor;

  /**
   * The storage for the cache
   */
  TOutput *m_Cache;

  /**
   * The bounds of the cache
   */
  TInput m_CacheBegin;
  
  /** The length of the cache */
  unsigned int m_CacheLength;


  /**
   * The functor
   */
  CachingUnaryFunctor<TInput,TOutput,TFunctor> m_CachingFunctor;
};

/**
 * \class CachingUnaryFunctor
 * \brief A functor that works with UnaryFunctorCache to return precomputed 
 * values
 */
template <class TInput, class TOutput, class TFunctor> 
class CachingUnaryFunctor 
{
public:
  // Typedef to the cache that updates this object
  typedef CachingUnaryFunctor<TInput,TOutput,TFunctor> Self;
  typedef UnaryFunctorCache<TInput,TOutput,TFunctor> CacheType;
  typedef typename itk::SmartPointer<CacheType> CachePointer;

  /** Perform an evaluation using the cache */
  TOutput operator()(const TInput &in) { return m_Parent->Evaluate(in); }

  /** Initialize with a cache object */
  CachingUnaryFunctor(CacheType *parent) 
    { m_Parent = parent; }

  /** Default constructor */
  CachingUnaryFunctor() 
    { m_Parent = NULL; }

  /** Comparison operator necessitated by ITK */
  bool operator == (const Self &z) const 
    { return m_Parent == z.m_Parent; }

  /** Comparison operator necessitated by ITK */
  bool operator != (const Self &z) const 
    { return !(*this == z); }

private:
  /** Pointer to the cache */
  CachePointer m_Parent;
};

#ifndef ITK_MANUAL_INSTANTIATION
#include "UnaryFunctorCache.txx"
#endif

#endif // __UnaryFunctorCache_h_
