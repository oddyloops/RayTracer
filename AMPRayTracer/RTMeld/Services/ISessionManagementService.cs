using RTMeld.DataTransport;
using RTMeld.Enums;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts that all session management services must fulfill
    /// </summary>
    public interface ISessionManagementService : IDataService
    {
        /// <summary>
        /// Logs into and starts a session for the specified user account within a non-blocking context
        /// </summary>
        /// <param name="user">User account containing log in credentials</param>
        /// <returns>A callback handle providing access to the user session token</returns>
        Task<IRTUserSession> LogInAsync(IRTUser user);

        /// <summary>
        /// Logs out of the current session within a non-blocking context`
        /// </summary>
        /// <param name="userSession">Session to be closed</param>
        /// <returns>A callback handle providing access to the  status code indicating result of the operation</returns>
        Task<StatusCode> LogOutAsync(IRTUserSession userSession);


    }
}
