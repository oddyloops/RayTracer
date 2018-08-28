using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts that all account management services must satisfy
    /// </summary>
    interface IAccountManagementService : IDataService
    {
       
        
        /// <summary>
        /// Creates a new user account for the application
        /// </summary>
        /// <param name="user">User account details</param>
        /// <returns>Status code indicating the result of the operation</returns>
        StatusCode CreateUserAccount(IRTUser user);

        /// <summary>
        /// Permanently closes a user account
        /// </summary>
        /// <param name="accountId">ID of the user account to be closed</param>
        /// <returns>Status code indicating the result of the operation</returns>
        StatusCode CloseUserAccount(object accountId);

        /// <summary>
        /// Suspends user account until it is reopened
        /// </summary>
        /// <param name="accountId">ID of the user account to be suspended</param>
        /// <returns>Status code indicating the result of the operation</returns>
        StatusCode SuspendUserAccount(object accountId);

        /// <summary>
        /// Reopens a previously suspended user account
        /// </summary>
        /// <param name="accountId">ID of the user account to be reopened</param>
        /// <returns>Status code indicating the result of the operation</returns>
        StatusCode ReopenUserAccount(object accountId);

        /// <summary>
        /// Initiates password recovery by username
        /// </summary>
        /// <param name="username">Account username</param>
        /// <returns>Status code indicating the result of the operation</returns>
        StatusCode PasswordRecoveryByUsername(string username);

        /// <summary>
        /// Initiates password recovery by email
        /// </summary>
        /// <param name="email">Account email</param>
        /// <returns>Status code indicating the result of the operation</returns>
        StatusCode PasswordRecoveryByEmail(string email);

        /// <summary>
        /// Resets password for user account
        /// </summary>
        /// <param name="user">User account containing new password</param>
        /// <returns>Status code indicating the result of the operation</returns>
        StatusCode ResetPassword(IRTUser user);

    }
}
