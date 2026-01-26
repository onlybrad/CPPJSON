#pragma once

#include <cassert>
#include <functional>

namespace CPPJSON {

template<typename TValue, typename TError = bool>
class Result {
    union {
        TValue m_value;
        TError m_error{};
    };
    bool m_success = false;

public:
    typedef std::function<void(TValue)>        SuccessCallback;
    typedef std::function<void(TValue&)>       SuccessRefCallback;
    typedef std::function<void(TError)>        ErrorCallback;
    typedef std::function<void()>              ErrorCallback2;
    
    Result ()                                          noexcept = default;
    Result (const Result<TValue, TError>&)             noexcept;
    Result (Result<TValue, TError>&&)                  noexcept;
    ~Result()                                          noexcept;
    Result &operator=(const Result<TValue, TError>&)   noexcept;
    Result &operator=(Result<TValue, TError>&&)        noexcept;
    TValue *operator->()                               noexcept;
    TValue *operator->()                         const noexcept;

    void setValue(TValue);
    void setError(TError);

    static Result fromValue(TValue);
    static Result fromError(TError);

    Result       &onSuccess   (SuccessCallback);
    Result       &onSuccessRef(SuccessRefCallback);
    Result       &onError     (ErrorCallback);
    Result       &onError     (ErrorCallback2);

    TValue        getValue () const noexcept;
    const TValue &getRef   () const noexcept;
    TValue       &getRef   ()       noexcept;
    TError        getError () const noexcept;
    bool          isSuccess() const noexcept;
};

template<typename TValue, typename TError>
class Result<TValue&, TError> {
    union {
        TValue *m_value;
        TError  m_error{};
    };
    bool m_success;

public:
    typedef std::function<void(TValue&)> SuccessCallback;
    typedef std::function<void(TError)>  ErrorCallback;
    typedef std::function<void()>        ErrorCallback2;

    Result ()                                          noexcept = default;
    Result (const Result<TValue&, TError>&)            noexcept;
    ~Result()                                          noexcept;
    Result &operator=(const Result<TValue&, TError>&)  noexcept;
    TValue *operator->()                               noexcept;
    TValue *operator->()                         const noexcept;

    void setRef(TValue&);
    void setError(TError);

    static Result fromRef(TValue&);
    static Result fromError(TError);

    Result &onSuccess(SuccessCallback);
    Result &onError  (ErrorCallback);
    Result &onError  (ErrorCallback2);
    TValue &getRef   () const noexcept;
    TError  getError () const noexcept;
    bool    isSuccess() const noexcept;
};

template<typename TValue, typename TError>
Result<TValue, TError>::~Result() noexcept {
    if(m_success) {
        m_value.~TValue();
    } else {
        m_error.~TError();
    }
}

template<typename TValue, typename TError>
Result<TValue&, TError>::~Result() noexcept {
    if(!m_success) {   
        m_error.~TError();
    }
}

template<typename TValue, typename TError>
Result<TValue, TError>::Result(const Result<TValue, TError> &result) noexcept :
m_success(result.m_success) {
    if(m_success) {
        m_value = result.m_value;
    } else {
        m_error = result.m_error;
    }
}

template<typename TValue, typename TError>
Result<TValue&, TError>::Result(const Result<TValue&, TError> &result) noexcept :
m_success(result.m_success) {
    if(m_success) {
        m_value = result.m_value;
    } else {
        m_error = result.m_error;
    }
}

template<typename TValue, typename TError>
Result<TValue, TError>::Result(Result<TValue, TError> &&result) noexcept : 
m_success(result.m_success) {
    if(m_success) {
        m_value = std::move(result.m_value);
    } else {
        m_error = std::move(result.m_error);
    }
}

template<typename TValue, typename TError>
Result<TValue, TError> &Result<TValue, TError>::operator=(const Result<TValue, TError> &result) noexcept {
    if(result.m_success) {
        setValue(result.m_value);
    } else {
        setError(result.m_error);
    }
    
    return *this;
}

template<typename TValue, typename TError>
Result<TValue&, TError> &Result<TValue&, TError>::operator=(const Result<TValue&, TError> &result) noexcept {
    if(result.m_success) {
        setRef(result.m_value);
    } else {
        setError(result.m_error);
    }
    
    return *this;
}

template<typename TValue, typename TError>
Result<TValue, TError> &Result<TValue, TError>::operator=(Result<TValue, TError> &&result) noexcept {
    if(this != &result) {
        if(result.m_success) {
            if(m_error) {
                m_error.~TError();
            }
            m_value = std::move(result.m_value);
        } else {
            if(!m_error) {
                m_value.~TValue();
            }
            m_error = std::move(result.m_error);
        }
        
        m_success = result.m_success;
    }

    return *this;
}

template<typename TValue, typename TError>
TValue *Result<TValue, TError>::operator->() noexcept {
    return &m_value;
};

template<typename TValue, typename TError>
TValue *Result<TValue, TError>::operator->() const noexcept {
    return &m_value;
}

template<typename TValue, typename TError>
void Result<TValue, TError>::setValue(const TValue value) {
    if(!m_success) {
        m_error.~TError();
    }

    m_value   = value;
    m_success = true;
}

template<typename TValue, typename TError>
void Result<TValue, TError>::setError(const TError error) {
    if(m_success) {
        m_value.~TValue();
    }

    m_error   = error;
    m_success = false;
}

template<typename TValue, typename TError>
void Result<TValue&, TError>::setRef(TValue &value) {
    if(!m_success) {
        m_error.~TError();
    }

    m_value   = &value;
    m_success = true;
}

template<typename TValue, typename TError>
void Result<TValue&, TError>::setError(const TError error) {
    m_error   = error;
    m_success = false;
}

template<typename TValue, typename TError>
TValue *Result<TValue&, TError>::operator->() noexcept {
    return m_value;
};

template<typename TValue, typename TError>
TValue *Result<TValue&, TError>::operator->() const noexcept {
    return m_value;
};

template<typename TValue, typename TError>
Result<TValue, TError> Result<TValue, TError>::fromValue(const TValue value) {
    Result result;
    result.m_success = true;
    new (&result.m_value) TValue();
    result.m_value = value;

    return result;
}

template<typename TValue, typename TError>
Result<TValue&, TError> Result<TValue&, TError>::fromRef(TValue &value) {
    Result result;
    result.m_success = true;
    result.m_value   = &value;

    return result;
}

template<typename TValue, typename TError>
Result<TValue, TError> Result<TValue, TError>::fromError(const TError error) {
    Result result;
    result.m_success = false;
    new (&result.m_error) TError();
    result.m_error = error;

    return result;
}

template<typename TValue, typename TError>
Result<TValue&, TError> Result<TValue&, TError>::fromError(const TError error) {
    Result result;
    result.m_success = false;
    new (&result.m_error) TError();
    result.m_error = error;

    return result;
}

template<typename TValue, typename TError>
Result<TValue, TError> &Result<TValue, TError>::onSuccess(SuccessCallback successCallback) {
    if(m_success) {
        successCallback(m_value);
    }
    return *this;
}

template <typename TValue, typename TError>
Result<TValue, TError> &Result<TValue, TError>::onSuccessRef(SuccessRefCallback successRefCallback) {
    if(m_success) {
        successRefCallback(m_value);
    }
    return *this;
}

template<typename TValue, typename TError>
Result<TValue&, TError> &Result<TValue&, TError>::onSuccess(SuccessCallback successCallback) {
    if(m_success) {
        successCallback(*m_value);
    }
    return *this;
}

template<typename TValue, typename TError>
Result<TValue, TError> &Result<TValue, TError>::onError(ErrorCallback errorCallback) {
    if(!m_success) {
        errorCallback(m_error);
    }
    return *this;
}

template<typename TValue, typename TError>
Result<TValue, TError> &Result<TValue, TError>::onError(ErrorCallback2 errorCallback) {
    if(!m_success) {
        errorCallback();
    }
    return *this;
}

template<typename TValue, typename TError>
Result<TValue&, TError> &Result<TValue&, TError>::onError(ErrorCallback errorCallback) {
    if(!m_success) {
        errorCallback(m_error);
    }
    return *this;
}

template<typename TValue, typename TError>
Result<TValue&, TError> &Result<TValue&, TError>::onError(ErrorCallback2 errorCallback) {
    if(!m_success) {
        errorCallback();
    }
    return *this;
}

template<typename TValue, typename TError>
TValue Result<TValue, TError>::getValue() const noexcept {
    return m_value;
}

template<typename TValue, typename TError>
const TValue &Result<TValue, TError>::getRef() const noexcept {
    return m_value;
}

template<typename TValue, typename TError>
TValue &Result<TValue, TError>::getRef() noexcept {
    return m_value;
}

template<typename TValue, typename TError>
TValue &Result<TValue&, TError>::getRef() const noexcept {
    return *m_value;
}

template<typename TValue, typename TError>
TError Result<TValue, TError>::getError() const noexcept {
    return m_error;
}

template<typename TValue, typename TError>
TError Result<TValue&, TError>::getError() const noexcept {
    return m_error;
}

template<typename TValue, class TError>
bool Result<TValue, TError>::isSuccess() const noexcept {
    return m_success;
}

template<typename TValue, class TError>
bool Result<TValue&, TError>::isSuccess() const noexcept {
    return m_success;
}

}