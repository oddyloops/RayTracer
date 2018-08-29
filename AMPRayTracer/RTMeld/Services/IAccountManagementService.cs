using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts that all account management services must satisfy
    /// </summary>
    public interface IAccountManagementService : IDataService
    {
       
        

        /// <summary>
        /// Creates a new user account for the application within a non-blocking context
        /// </summary>
        /// <param name="user">User account details</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> CreateUserAccountAsync(IRTUser user);


        /// <summary>
        /// Permanently closes a user account within a non-blocking context
        /// </summary>
        /// <param name="user">User account details</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> CloseUserAccountAsync(IRTUser user);


        /// <summary>
        /// Suspends user account until it is reopened within a non-blocking context
        /// </summary>
        /// <param name="user">User account details</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> SuspendUserAccountAsync(IRTUser user);

        /// <summary>
        /// Reopens a previously suspended user account within a non-blocking context
        /// </summary>
        /// <param name="user">User account details</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> ReopenUserAccountAsync(IRTUser user);

        /// <summary>
        /// Initiates password recovery by username within a non-blocking context
        /// </summary>
        /// <param name="username">Account username</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> PasswordRecoveryByUsernameAsync(string username);

        /// <summary>
        /// Initiates password recovery by email within a non-blocking context
        /// </summary>
        /// <param name="email">Account email</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> PasswordRecoveryByEmailAsync(string email);

        /// <summary>
        /// Resets password for user account  within a non-blocking context
        /// </summary>
        /// <param name="user">User account containing new password</param>
        /// <returns>A callback handle providing access to the status code indicating the result of the operation</returns>
        Task<StatusCode> ResetPasswordAsync(IRTUser user);

    }
}
