/*
 * Copyright (C) 2011 Igalia S.L.
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

#ifndef WebKitWebContextPrivate_h
#define WebKitWebContextPrivate_h

#include "CustomProtocolManagerProxy.h"
#include "DownloadProxy.h"
#include "WebKitPrivate.h"
#include "WebKitUserContentManager.h"
#include "WebKitWebContext.h"
#include "WebProcessPool.h"
#include <WebCore/ResourceRequest.h>

WebKit::WebProcessPool* webkitWebContextGetProcessPool(WebKitWebContext*);
WebKitDownload* webkitWebContextGetOrCreateDownload(WebKit::DownloadProxy*);
WebKitDownload* webkitWebContextStartDownload(WebKitWebContext*, const char* uri, WebKit::WebPageProxy*);
void webkitWebContextRemoveDownload(WebKit::DownloadProxy*);
void webkitWebContextDownloadStarted(WebKitWebContext*, WebKitDownload*);
void webkitWebContextStartLoadingCustomProtocol(WebKitWebContext*, uint64_t customProtocolID, const WebCore::ResourceRequest&, WebKit::CustomProtocolManagerProxy&);
void webkitWebContextStopLoadingCustomProtocol(WebKitWebContext*, uint64_t customProtocolID);
void webkitWebContextInvalidateCustomProtocolRequests(WebKitWebContext*, WebKit::CustomProtocolManagerProxy&);
void webkitWebContextDidFinishLoadingCustomProtocol(WebKitWebContext*, uint64_t customProtocolID);
bool webkitWebContextIsLoadingCustomProtocol(WebKitWebContext*, uint64_t customProtocolID);
void webkitWebContextCreatePageForWebView(WebKitWebContext*, WebKitWebView*, WebKitUserContentManager*, WebKitWebView*);
void webkitWebContextWebViewDestroyed(WebKitWebContext*, WebKitWebView*);
WebKitWebView* webkitWebContextGetWebViewForPage(WebKitWebContext*, WebKit::WebPageProxy*);
GVariant* webkitWebContextInitializeWebExtensions(WebKitWebContext*);

#endif // WebKitWebContextPrivate_h
