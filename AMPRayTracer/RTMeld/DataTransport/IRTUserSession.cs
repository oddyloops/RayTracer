﻿using System;
using RTMeld.DataAccess;
using RTMeld.Attributes;


namespace RTMeld.DataTransport
{
    public interface IRTUserSession
    {
        [Map("user_id,UserId")]
        Guid UserId { get; set; }

        [Map("sessionId,sessionid,session_id")]
        byte[] SessionId { get; set; }

        [Map("log_in_time,login_time")]
        DateTime LogInTime { get; set; }
        #region Parent(s)
        IRTUser User { get; set; }
        #endregion
    }
}
