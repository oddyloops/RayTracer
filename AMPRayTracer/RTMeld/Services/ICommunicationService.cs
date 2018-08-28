

using RTMeld.Enums;
using System.Collections.Generic;

namespace RTMeld.Services
{
    /// <summary>
    /// Contracts that all communication services must satisfy
    /// </summary>
    public interface ICommunicationService : IService
    {
        /// <summary>
        /// Sends out an email to a single recipient
        /// </summary>
        /// <param name="recipient">Recipient email</param>
        /// <param name="subject">Email subject</param>
        /// <param name="body">Email content</param>
        /// <returns></returns>
        StatusCode SendEmail(string recipient, string subject, string body);

        /// <summary>
        /// Sends out an email to multiple recipients, with a list of copies and blind copies
        /// </summary>
        /// <param name="recipients">Primary email recipients</param>
        /// <param name="ccs">Copy emails</param>
        /// <param name="bccs">Blind copy emails</param>
        /// <param name="subject">Email subject</param>
        /// <param name="body">Email content</param>
        /// <returns></returns>
        StatusCode SendEmail(IList<string> recipients, IList<string> ccs, IList<string> bccs, string subject, string body);
    }
}
