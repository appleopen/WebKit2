/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "APIObject.h"
#include <WebCore/AuthenticationChallenge.h>
#include <wtf/WeakPtr.h>

namespace IPC {
class Connection;
}

namespace WebKit {

class AuthenticationDecisionListener;
class ChildProcessProxy;
class SecKeyProxyStore;
class WebCredential;
class WebProtectionSpace;

class AuthenticationChallengeProxy : public API::ObjectImpl<API::Object::Type::AuthenticationChallenge> {
public:
    static Ref<AuthenticationChallengeProxy> create(WebCore::AuthenticationChallenge&& authenticationChallenge, uint64_t challengeID, IPC::Connection* connection)
    {
        return adoptRef(*new AuthenticationChallengeProxy(WTFMove(authenticationChallenge), challengeID, connection));
    }
    
    ~AuthenticationChallengeProxy();
    
    void useCredential(WebCredential*);
    void cancel();
    void performDefaultHandling();
    void rejectProtectionSpaceAndContinue();

    AuthenticationDecisionListener* listener() const { return m_listener.get(); }
    WebCredential* proposedCredential() const;
    WebProtectionSpace* protectionSpace() const;
    int previousFailureCount() const { return m_coreAuthenticationChallenge.previousFailureCount(); }
    const WebCore::AuthenticationChallenge& core() { return m_coreAuthenticationChallenge; }

#if HAVE(SEC_KEY_PROXY)
    void setSecKeyProxyStore(SecKeyProxyStore&);
#endif

private:
    AuthenticationChallengeProxy(WebCore::AuthenticationChallenge&&, uint64_t challengeID, IPC::Connection*);

#if HAVE(SEC_KEY_PROXY)
    void sendClientCertificateCredentialOverXpc(uint64_t challengeID, const WebCore::Credential&) const;
#endif

    WebCore::AuthenticationChallenge m_coreAuthenticationChallenge;
    uint64_t m_challengeID;
    RefPtr<IPC::Connection> m_connection;
    RefPtr<AuthenticationDecisionListener> m_listener;
    mutable RefPtr<WebCredential> m_webCredential;
    mutable RefPtr<WebProtectionSpace> m_webProtectionSpace;

#if HAVE(SEC_KEY_PROXY)
    WeakPtr<SecKeyProxyStore> m_secKeyProxyStore;
#endif
};

} // namespace WebKit
