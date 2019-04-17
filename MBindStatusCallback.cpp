// MBindStatusCallback.cpp --- progress info class
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include "MBindStatusCallback.hpp"

/*static*/ MBindStatusCallback *MBindStatusCallback::Create()
{
    return new MBindStatusCallback;
}

MBindStatusCallback::MBindStatusCallback() :
    m_nRefCount(1),
    m_bCompleted(FALSE),
    m_bCancelled(FALSE)
{
}

MBindStatusCallback::~MBindStatusCallback()
{
}

BOOL MBindStatusCallback::IsCancelled() const
{
    return m_bCancelled;
}

BOOL MBindStatusCallback::IsCompleted() const
{
    return m_bCompleted;
}

// IUnknown interface
STDMETHODIMP MBindStatusCallback::QueryInterface(REFIID riid, void **ppvObj)
{
    if (!ppvObj)
        return E_POINTER;

    *ppvObj = NULL;

    if (riid == __uuidof(IUnknown) || riid == __uuidof(IBindStatusCallback))
    {
        *ppvObj = static_cast<IBindStatusCallback *>(this);
    }
    else
    {
        return E_NOINTERFACE;
    }

    AddRef();

    return S_OK;
}

STDMETHODIMP_(ULONG) MBindStatusCallback::AddRef()
{
    return ++m_nRefCount;
}

STDMETHODIMP_(ULONG) MBindStatusCallback::Release()
{
    --m_nRefCount;
    if (m_nRefCount == 0)
    {
        delete this;
        return 0;
    }
    return m_nRefCount;
}

// IBindStatusCallback interface
STDMETHODIMP MBindStatusCallback::OnStartBinding(DWORD dwReserved, IBinding *pib)
{
    return S_OK;
}

STDMETHODIMP MBindStatusCallback::GetPriority(LONG *pnPriority)
{
    return E_NOTIMPL;
}

STDMETHODIMP MBindStatusCallback::OnLowResource(DWORD reserved)
{
    return E_NOTIMPL;
}

STDMETHODIMP MBindStatusCallback::OnProgress(
    ULONG ulProgress,
    ULONG ulProgressMax,
    ULONG ulStatusCode,
    LPCWSTR szStatusText)
{
    m_ulProgress = ulProgress;
    m_ulProgressMax = ulProgressMax;
    m_ulStatusCode = ulStatusCode;
    if (szStatusText)
        m_strStatus = szStatusText;
    if (ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA ||
        ulStatusCode == BINDSTATUS_ENDDOWNLOADCOMPONENTS)
    {
        m_bCompleted = TRUE;
        return S_OK;
    }
    if (m_bCancelled)
    {
        printf("Cancelled\n");
        return E_ABORT;
    }
    return S_OK;
}

void MBindStatusCallback::SetCancelled()
{
    m_bCancelled = TRUE;
}

STDMETHODIMP MBindStatusCallback::OnStopBinding(HRESULT hresult, LPCWSTR szError)
{
    return S_OK;
}

STDMETHODIMP MBindStatusCallback::GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
{
    *grfBINDF = BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE |
                BINDF_GETNEWESTVERSION | BINDF_NOWRITECACHE;
    return S_OK;
}

STDMETHODIMP MBindStatusCallback::OnDataAvailable(
    DWORD grfBSCF,
    DWORD dwSize,
    FORMATETC *pformatetc,
    STGMEDIUM *pstgmed)
{
    return S_OK;
}

STDMETHODIMP MBindStatusCallback::OnObjectAvailable(REFIID riid, IUnknown *punk)
{
    return S_OK;
}
