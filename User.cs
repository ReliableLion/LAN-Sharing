using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LanSharing
{
    class User
    {

        private string username_;
        private string avatar_;

        public User(string username, string avatar) {
            username_ = username;
            avatar_ = avatar;
        }

        public string get_username() {
            return username_;
        }

        public string get_avatar() {
            return avatar_;
        }
    }
}
