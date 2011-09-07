/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

Modified version of qshareddata.h from Qt 4.7.3 for MITK.
Original copyright (c) Nokia Corporation. Usage covered by the
GNU Lesser General Public License version 2.1
(http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html) and the Nokia Qt
LGPL Exception version 1.1 (file LGPL_EXCEPTION.txt in Qt 4.7.3 package).

=========================================================================*/

#ifndef MITKSHAREDDATA_H
#define MITKSHAREDDATA_H

#include <algorithm>
#include <utility>

#include <MitkExports.h>
#include "mitkAtomicInt.h"

namespace mitk {

template <class T> class SharedDataPointer;

class MITK_CORE_EXPORT SharedData
{
public:
  mutable AtomicInt ref;

  inline SharedData() : ref(0) { }
  inline SharedData(const SharedData&) : ref(0) { }

private:
  // using the assignment operator would lead to corruption in the ref-counting
  SharedData& operator=(const SharedData&);
};

template <class T> class SharedDataPointer
{
public:
  typedef T Type;
  typedef T* pointer;

  inline void Detach() { if (d && d->ref != 1) Detach_helper(); }
  inline T& operator*() { Detach(); return *d; }
  inline const T& operator*() const { return *d; }
  inline T* operator->() { Detach(); return d; }
  inline const T* operator->() const { return d; }
  inline operator T*() { Detach(); return d; }
  inline operator const T*() const { return d; }
  inline T* Data() { Detach(); return d; }
  inline const T* Data() const { return d; }
  inline const T* ConstData() const { return d; }

  inline bool operator==(const SharedDataPointer<T>& other) const { return d == other.d; }
  inline bool operator!=(const SharedDataPointer<T>& other) const { return d != other.d; }

  inline SharedDataPointer() { d = 0; }
  inline ~SharedDataPointer() { if (d && !d->ref.Deref()) delete d; }

  explicit SharedDataPointer(T* data);
  inline SharedDataPointer(const SharedDataPointer<T>& o) : d(o.d) { if (d) d->ref.Ref(); }

  inline SharedDataPointer<T> & operator=(const SharedDataPointer<T>& o)
  {
    if (o.d != d)
    {
      if (o.d)
        o.d->ref.Ref();
      T *old = d;
      d = o.d;
      if (old && !old->ref.Deref())
        delete old;
    }
    return *this;
  }

  inline SharedDataPointer &operator=(T *o)
  {
    if (o != d)
    {
      if (o)
        o->ref.Ref();
      T *old = d;
      d = o;
      if (old && !old->ref.Deref())
        delete old;
    }
    return *this;
  }

  inline bool operator!() const { return !d; }

  inline void Swap(SharedDataPointer& other)
  {
    using std::swap;
    swap(d, other.d);
  }

protected:
  T* Clone();

private:
  void Detach_helper();

  T *d;
};

template <class T> class ExplicitlySharedDataPointer
{
public:
  typedef T Type;
  typedef T* pointer;

  inline T& operator*() const { return *d; }
  inline T* operator->() { return d; }
  inline T* operator->() const { return d; }
  inline T* Data() const { return d; }
  inline const T* ConstData() const { return d; }

  inline void Detach() { if (d && d->ref != 1) Detach_helper(); }

  inline void Reset()
  {
    if(d && !d->ref.Deref())
      delete d;

    d = 0;
  }

  inline operator bool () const { return d != 0; }

  inline bool operator==(const ExplicitlySharedDataPointer<T>& other) const { return d == other.d; }
  inline bool operator!=(const ExplicitlySharedDataPointer<T>& other) const { return d != other.d; }
  inline bool operator==(const T* ptr) const { return d == ptr; }
  inline bool operator!=(const T* ptr) const { return d != ptr; }

  inline ExplicitlySharedDataPointer() { d = 0; }
  inline ~ExplicitlySharedDataPointer() { if (d && !d->ref.Deref()) delete d; }

  explicit ExplicitlySharedDataPointer(T* data);
  inline ExplicitlySharedDataPointer(const ExplicitlySharedDataPointer<T> &o)
    : d(o.d) { if (d) d->ref.Ref(); }

  template<class X>
  inline ExplicitlySharedDataPointer(const ExplicitlySharedDataPointer<X>& o)
    : d(static_cast<T*>(o.Data()))
  {
    if(d)
      d->ref.Ref();
  }

  inline ExplicitlySharedDataPointer<T>& operator=(const ExplicitlySharedDataPointer<T>& o)
  {
    if (o.d != d)
    {
      if (o.d)
        o.d->ref.Ref();
      T *old = d;
      d = o.d;
      if (old && !old->ref.Deref())
        delete old;
    }
    return *this;
  }

  inline ExplicitlySharedDataPointer& operator=(T* o)
  {
    if (o != d)
    {
      if (o)
        o->ref.Ref();
      T *old = d;
      d = o;
      if (old && !old->ref.Deref())
        delete old;
    }
    return *this;
  }

  inline bool operator!() const { return !d; }

  inline void Swap(ExplicitlySharedDataPointer& other)
  {
    using std::swap;
    swap(d, other.d);
  }

protected:
  T* Clone();

private:
  void Detach_helper();

  T *d;
};


template <class T>
SharedDataPointer<T>::SharedDataPointer(T* adata) : d(adata)
{ if (d) d->ref.Ref(); }

template <class T>
T* SharedDataPointer<T>::Clone()
{
  return new T(*d);
}

template <class T>
void SharedDataPointer<T>::Detach_helper()
{
  T *x = Clone();
  x->ref.Ref();
  if (!d->ref.Deref())
    delete d;
  d = x;
}

template <class T>
T* ExplicitlySharedDataPointer<T>::Clone()
{
  return new T(*d);
}

template <class T>
void ExplicitlySharedDataPointer<T>::Detach_helper()
{
  T *x = Clone();
  x->ref.Ref();
  if (!d->ref.Deref())
    delete d;
  d = x;
}

template <class T>
ExplicitlySharedDataPointer<T>::ExplicitlySharedDataPointer(T* adata)
  : d(adata)
{ if (d) d->ref.Ref(); }

template <class T>
void swap(mitk::SharedDataPointer<T>& p1, mitk::SharedDataPointer<T>& p2)
{ p1.Swap(p2); }

template <class T>
void swap(mitk::ExplicitlySharedDataPointer<T>& p1, mitk::ExplicitlySharedDataPointer<T>& p2)
{ p1.Swap(p2); }

}

#endif // MITKSHAREDDATA_H