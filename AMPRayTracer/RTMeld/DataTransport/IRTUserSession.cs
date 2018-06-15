using System;
using RTMeld.Attributes;


namespace RTMeld.DataTransport
{
    public interface IRTUserSession
    {
        [Key]
        [Map("user_id,userId")]
        Guid UserId { get; set; }

        [Map("sessionId,sessionid,session_id")]
        byte[] SessionId { get; set; }

        [Map("log_in_time,login_time")]
        DateTime LogInTime { get; set; }
    
    }
}
