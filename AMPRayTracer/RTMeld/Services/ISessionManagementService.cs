using RTMeld.DataTransport;
using RTMeld.Enums;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts that all session management services must fulfill
    /// </summary>
    public interface ISessionManagementService : IDataService
    {
        /// <summary>
        /// Logs into and starts a session for the specified user account
        /// </summary>
        /// <param name="user">User account containing log in credentials</param>
        /// <returns>A user session token</returns>
        IRTUserSession LogIn(IRTUser user);

        /// <summary>
        /// Logs out of the current session
        /// </summary>
        /// <param name="userSession">Session to be closed</param>
        /// <returns>Status code indicating result of the operation</returns>
        StatusCode LogOut(IRTUserSession userSession);


    }
}
