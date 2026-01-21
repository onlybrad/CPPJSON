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

    Result(bool success) noexcept;

public:
    typedef std::function<void(TValue)> SuccessCallback;
    typedef std::function<void(TError)> ErrorCallback;
    typedef std::function<void()>       ErrorCallback2;
    
    Result ()                                          noexcept = default;
    Result (const Result<TValue, TError>&)             noexcept;
    Result (Result<TValue, TError>&&)                  noexcept;
    ~Result()                                          noexcept;
    Result &operator=(const Result<TValue, TError>&)   noexcept;
    Result &operator=(Result<TValue, TError>&&)        noexcept;
    TValue *operator->()                               noexcept;
    TValue *operator->()                         const noexcept;

    static Result fromValue(TValue);
    static Result fromError(TError);

    Result &onSuccess(SuccessCallback);
    Result &onError  (ErrorCallback);
    Result &onError  (ErrorCallback2);
    TValue getValue  () const noexcept;
    TError getError  () const noexcept;
    bool   isSuccess () const noexcept;
};

template<typename TValue, typename TError>
class Result<TValue&, TError> {
    union {
        TValue *m_value;
        TError  m_error{};
    };
    bool m_success;

    Result(bool success) noexcept;

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

    static Result fromRef(TValue&);
    static Result fromError(TError);

    Result &onSuccess(SuccessCallback);
    Result &onError  (ErrorCallback);
    Result &onError  (ErrorCallback2);
    TValue &getRef   () const noexcept;
    TError  getError () const noexcept;
    bool    isSuccess() const noexcept;
};

template<typename TValue, class TError>
Result<TValue, TError>::Result(bool success) noexcept :
m_success(success)
{}

template<typename TValue, class TError>
Result<TValue&, TError>::Result(bool success) noexcept :
m_success(success)
{}

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
    m_success = result.m_success;
    if(m_success) {
        m_value = result.m_value;
    } else {
        m_error = result.m_error;
    }

    return *this;
}

template<typename TValue, typename TError>
Result<TValue&, TError> &Result<TValue&, TError>::operator=(const Result<TValue&, TError> &result) noexcept {
    m_success = result.m_success;
    if(m_success) {
        m_value = result.m_value;
    } else {
        m_error = result.m_error;
    }

    return *this;
}

template<typename TValue, typename TError>
Result<TValue, TError> &Result<TValue, TError>::operator=(Result<TValue, TError> &&result) noexcept {
    if(this != &result) {
        m_success = result.m_success;
        if(m_success) {
            m_value = std::move(result.m_value);
        } else {
            m_error = std::move(result.m_error);
        }        
    }

    return *this;
}

template <typename TValue, typename TError>
TValue *Result<TValue, TError>::operator->() noexcept {
    return &m_value;
};

template <typename TValue, typename TError>
TValue *Result<TValue, TError>::operator->() const noexcept {
    return &m_value;
};

template <typename TValue, typename TError>
TValue *Result<TValue&, TError>::operator->() noexcept {
    return m_value;
};

template <typename TValue, typename TError>
TValue *Result<TValue&, TError>::operator->() const noexcept {
    return m_value;
};

template<typename TValue, typename TError>
Result<TValue, TError> Result<TValue, TError>::fromValue(TValue value) {
    Result result(true);
    result.m_value = value;

    return result;
}

template<typename TValue, typename TError>
Result<TValue&, TError> Result<TValue&, TError>::fromRef(TValue &value) {
    Result result(true);
    result.m_value = &value;

    return result;
}

template<typename TValue, typename TError>
Result<TValue, TError> Result<TValue, TError>::fromError(TError error) {
    Result result(false);
    result.m_error = error;

    return result;
}

template<typename TValue, typename TError>
Result<TValue&, TError> Result<TValue&, TError>::fromError(TError error) {
    Result result(false);
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
TValue& Result<TValue&, TError>::getRef() const noexcept {
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