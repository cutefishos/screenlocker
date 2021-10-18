/*
 * Copyright (C) 2021 CutefishOS Team
 * Copyright (C) 2014 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author : Arto Jalkanen <ajalkane@gmail.com>
 */

#include "pamauthentication.h"
#include <QDebug>

// PAM
#include <security/pam_appl.h>

PamAuthentication::PamAuthentication(QObject *parent)
    : QObject(parent)
{
    m_userName = std::getenv("USER");
}

bool PamAuthentication::verify(const QString &password)
{
    pam_handle *pamHandle = 0;

    if (!initPam(&pamHandle)) {
        qDebug() << Q_FUNC_INFO << "Pam init failed";
        return false;
    }

    m_password = password;

    int status = pam_authenticate(pamHandle, 0);
    qDebug() << Q_FUNC_INFO << "Pam authenticate status" << status << pam_strerror(pamHandle, status);
    if (status == PAM_SUCCESS) {
        status = validateAccount(pamHandle);
    }
    pam_end(pamHandle, status);

    m_password.clear();

    return status == PAM_SUCCESS;
}

bool PamAuthentication::initPam(pam_handle **pamHandle)
{
    pam_conv conversation;
    conversation.conv = pamConv;
    conversation.appdata_ptr = static_cast<void *>(this);

    return pam_start("cutefishos", m_userName.toLocal8Bit().data(),
                     &conversation, pamHandle) == PAM_SUCCESS;
}

int PamAuthentication::validateAccount(pam_handle *pamHandle)
{
    // This makes sure account and password are still valid
    int status = pam_acct_mgmt(pamHandle, 0);
    qDebug() << Q_FUNC_INFO << "pam_acct_mgmt: " << status << pam_strerror(pamHandle, status);
    // Placeholders for some common errors
    // IMPROVE: it'd be good to let user know reason for failure
    switch (status) {
    case PAM_SUCCESS:
        break;
    case PAM_USER_UNKNOWN:
        break;
    case PAM_ACCT_EXPIRED:
        break;
    case PAM_NEW_AUTHTOK_REQD:
        break;
    }
    return status;
}

int PamAuthentication::pamConv(int num_msg, const pam_message **msg, pam_response **resp, void *appdata_ptr)
{
    if (num_msg <= 0) {
        return PAM_CONV_ERR;
    }

    *resp = static_cast<pam_response *>(calloc(num_msg, sizeof(pam_response)));

    PamAuthentication *self = static_cast<PamAuthentication *>(appdata_ptr);

    for (int count = 0; count < num_msg; ++count) {
        switch (msg[count]->msg_style) {
        case PAM_PROMPT_ECHO_ON: {
            qDebug() << Q_FUNC_INFO << "PAM_PROMPT_ECHO_ON received";
            resp[count]->resp = strdup(self->m_password.toLocal8Bit().data());
            resp[count]->resp_retcode = 0;
            break;
        }
        case PAM_PROMPT_ECHO_OFF: {
            qDebug() << Q_FUNC_INFO << "PAM_PROMPT_ECHO_OFF received";
            resp[count]->resp = strdup(self->m_password.toLocal8Bit().data());
            resp[count]->resp_retcode = 0;
            break;
        }
        case PAM_TEXT_INFO: {
            QString message(msg[count]->msg);
            qDebug() << Q_FUNC_INFO << "PAM_TEXT_INFO received" << message;
            break;
        }
        case PAM_AUTHTOK: {
            qDebug() << Q_FUNC_INFO << "PAM_AUTHTOK received";
            break;
        }
        default: {
            qDebug() << Q_FUNC_INFO << "Other PAM msg received: " << msg[count]->msg_style;
        }
        }
    }

    return PAM_SUCCESS;
}
