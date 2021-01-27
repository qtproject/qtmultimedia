/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qopenslesdeviceinfo_p.h"

#include "qopenslesengine_p.h"

QT_BEGIN_NAMESPACE

QOpenSLESDeviceInfo::QOpenSLESDeviceInfo(const QByteArray &device, QAudio::Mode mode)
    : QAudioDeviceInfoPrivate(device, mode),
      m_engine(QOpenSLESEngine::instance())
{
}

bool QOpenSLESDeviceInfo::isFormatSupported(const QAudioFormat &format) const
{
    QOpenSLESDeviceInfo *that = const_cast<QOpenSLESDeviceInfo*>(this);
    return that->supportedSampleRates().contains(format.sampleRate())
            && that->supportedChannelCounts().contains(format.channelCount())
            && that->supportedSampleSizes().contains(format.sampleSize())
            && that->supportedByteOrders().contains(format.byteOrder())
            && that->supportedSampleTypes().contains(format.sampleType());
}

QAudioFormat QOpenSLESDeviceInfo::preferredFormat() const
{
    QAudioFormat format;
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(QOpenSLESEngine::getOutputValue(QOpenSLESEngine::SampleRate, 48000));
    format.setChannelCount(mode == QAudio::AudioInput ? 1 : 2);
    return format;
}

QString QOpenSLESDeviceInfo::deviceName() const
{
    return id;
}

QList<int> QOpenSLESDeviceInfo::supportedSampleRates() const
{
    return m_engine->supportedSampleRates(mode);
}

QList<int> QOpenSLESDeviceInfo::supportedChannelCounts() const
{
    return m_engine->supportedChannelCounts(mode);
}

QList<int> QOpenSLESDeviceInfo::supportedSampleSizes() const
{
    if (mode == QAudio::AudioInput)
        return QList<int>() << 16;
    else
        return QList<int>() << 8 << 16;
}

QList<QAudioFormat::Endian> QOpenSLESDeviceInfo::supportedByteOrders() const
{
    return QList<QAudioFormat::Endian>() << QAudioFormat::LittleEndian;
}

QList<QAudioFormat::SampleType> QOpenSLESDeviceInfo::supportedSampleTypes() const
{
    return QList<QAudioFormat::SampleType>() << QAudioFormat::SignedInt;
}

QT_END_NAMESPACE