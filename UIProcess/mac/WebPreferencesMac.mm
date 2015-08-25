/*
 * Copyright (C) 2010, 2011 Apple Inc. All rights reserved.
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

#import "config.h"
#import "WebPreferences.h"

#import "StringUtilities.h"
#import "WebPreferencesKeys.h"
#import <wtf/text/StringConcatenate.h>

namespace WebKit {

static inline NSString* makeKey(NSString *identifier, NSString *keyPrefix, NSString *key)
{
    ASSERT(identifier.length);
    ASSERT(keyPrefix);
    ASSERT(key);

    return [NSString stringWithFormat:@"%@%@%@", identifier, keyPrefix, key];
}

static void setStringValueIfInUserDefaults(const String& identifier, const String& keyPrefix, const String& key, WebPreferencesStore& store)
{
    id object = [[NSUserDefaults standardUserDefaults] objectForKey:makeKey(identifier, keyPrefix, key)];
    if (!object)
        return;
    if (![object isKindOfClass:[NSString class]])
        return;

    store.setStringValueForKey(key, (NSString *)object);
}

static void setBoolValueIfInUserDefaults(const String& identifier, const String& keyPrefix, const String& key, WebPreferencesStore& store)
{
    id object = [[NSUserDefaults standardUserDefaults] objectForKey:makeKey(identifier, keyPrefix, key)];
    if (!object)
        return;
    if (![object respondsToSelector:@selector(boolValue)])
        return;

    store.setBoolValueForKey(key, [object boolValue]);
}

static void setUInt32ValueIfInUserDefaults(const String& identifier, const String& keyPrefix, const String& key, WebPreferencesStore& store)
{
    id object = [[NSUserDefaults standardUserDefaults] objectForKey:makeKey(identifier, keyPrefix, key)];
    if (!object)
        return;
    if (![object respondsToSelector:@selector(intValue)])
        return;

    store.setUInt32ValueForKey(key, [object intValue]);
}

static void setDoubleValueIfInUserDefaults(const String& identifier, const String& keyPrefix, const String& key, WebPreferencesStore& store)
{
    id object = [[NSUserDefaults standardUserDefaults] objectForKey:makeKey(identifier, keyPrefix, key)];
    if (!object)
        return;
    if (![object respondsToSelector:@selector(doubleValue)])
        return;

    store.setDoubleValueForKey(key, [object doubleValue]);
}


static id debugUserDefaultsValue(NSString *identifier, NSString *keyPrefix, NSString *globalDebugKeyPrefix, NSString *key)
{
    ASSERT(keyPrefix);
    ASSERT(key);

    NSUserDefaults *standardUserDefaults = [NSUserDefaults standardUserDefaults];
    id object = nil;

    if (identifier.length)
        object = [standardUserDefaults objectForKey:[NSString stringWithFormat:@"%@%@%@", identifier, keyPrefix, key]];

    if (!object) {
        // Allow debug preferences to be set globally, using the debug key prefix.
        object = [standardUserDefaults objectForKey:[globalDebugKeyPrefix stringByAppendingString:key]];
    }

    return object;
}

static void setDebugBoolValueIfInUserDefaults(const String& identifier, const String& keyPrefix, const String& globalDebugKeyPrefix, const String& key, WebPreferencesStore& store)
{
    id object = debugUserDefaultsValue(identifier, keyPrefix, globalDebugKeyPrefix, key);
    if (!object)
        return;
    if (![object respondsToSelector:@selector(boolValue)])
        return;

    store.setBoolValueForKey(key, [object boolValue]);
}

void WebPreferences::platformInitializeStore()
{
#define INITIALIZE_DEBUG_PREFERENCE_FROM_NSUSERDEFAULTS(KeyUpper, KeyLower, TypeName, Type, DefaultValue) \
    setDebug##TypeName##ValueIfInUserDefaults(m_identifier, m_keyPrefix, m_globalDebugKeyPrefix, WebPreferencesKey::KeyLower##Key(), m_store);

    FOR_EACH_WEBKIT_DEBUG_PREFERENCE(INITIALIZE_DEBUG_PREFERENCE_FROM_NSUSERDEFAULTS)

#undef INITIALIZE_DEBUG_PREFERENCE_FROM_NSUSERDEFAULTS

    if (!m_identifier)
        return;

#define INITIALIZE_PREFERENCE_FROM_NSUSERDEFAULTS(KeyUpper, KeyLower, TypeName, Type, DefaultValue) \
    set##TypeName##ValueIfInUserDefaults(m_identifier, m_keyPrefix, WebPreferencesKey::KeyLower##Key(), m_store);

    FOR_EACH_WEBKIT_PREFERENCE(INITIALIZE_PREFERENCE_FROM_NSUSERDEFAULTS)

#undef INITIALIZE_PREFERENCE_FROM_NSUSERDEFAULTS
}

void WebPreferences::platformUpdateStringValueForKey(const String& key, const String& value)
{
    if (!m_identifier)
        return;

    [[NSUserDefaults standardUserDefaults] setObject:nsStringFromWebCoreString(value) forKey:makeKey(m_identifier, m_keyPrefix, key)];
}

void WebPreferences::platformUpdateBoolValueForKey(const String& key, bool value)
{
    if (!m_identifier)
        return;

    [[NSUserDefaults standardUserDefaults] setBool:value forKey:makeKey(m_identifier, m_keyPrefix, key)];
}

void WebPreferences::platformUpdateUInt32ValueForKey(const String& key, uint32_t value)
{
    if (!m_identifier)
        return;

    [[NSUserDefaults standardUserDefaults] setInteger:value forKey:makeKey(m_identifier, m_keyPrefix, key)];
}

void WebPreferences::platformUpdateDoubleValueForKey(const String& key, double value)
{
    if (!m_identifier)
        return;

    [[NSUserDefaults standardUserDefaults] setDouble:value forKey:makeKey(m_identifier, m_keyPrefix, key)];
}

void WebPreferences::platformUpdateFloatValueForKey(const String& key, float value)
{
    if (!m_identifier)
        return;

    [[NSUserDefaults standardUserDefaults] setFloat:value forKey:makeKey(m_identifier, m_keyPrefix, key)];
}

} // namespace WebKit
