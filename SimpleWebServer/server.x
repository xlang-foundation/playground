#
# Copyright (C) 2024 The XLang Foundation
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# <END>


from xlang_http import http
from xlang_os import fs
from xlang_sqlite import sqlite
from xlang_yaml import yaml
import time

# Initialize database
sqlite.UseDatabase("simple_userdb")

# Create users table if it doesn't exist
def create_users_table():
    pushWritepad(sqlite)
    
    # Check if the 'users' table already exists
    %users_table_if = SELECT name FROM sqlite_master WHERE type='table' AND name='users';
    table_exists = users_table_if.fetch()
    
    # Create the table if it doesn't exist
    if table_exists == None:
        %CREATE TABLE users (\
            username TEXT PRIMARY KEY,\
            password TEXT,\
            display_name TEXT,\
            last_login TEXT\
        );
        
        # Create a default admin user
        %INSERT INTO users (username, password, display_name, last_login) \
        VALUES ('admin', 'admin123', 'Administrator', '');
    
    popWritepad()

# Create a new HTTP server instance
srv = http.Server()
srv.SupportStaticFiles = True
srv.StaticRoots = ["./"]
srv.StaticIndexFile = "index.html"

# Initialize database
create_users_table()

# API: User Registration
@srv.route("/api/register")
def register_user():
    body = req.body
    jsonBody = yaml.loads(body)
    
    username = jsonBody["username"]
    password = jsonBody["password"]
    display_name = jsonBody["display_name"]
    
    pushWritepad(sqlite)
    
    # Check if username already exists
    %user_check = SELECT username FROM users WHERE username = ${username};
    existing_user = user_check.fetch()
    
    if existing_user != None:
        popWritepad()
        return [str({"success": False, "message": "Username already exists"}, format=True), "text/json"]
    
    # Insert new user
    %INSERT INTO users (username, password, display_name, last_login) \
    VALUES (${username}, ${password}, ${display_name}, '');
    
    popWritepad()
    return [str({"success": True, "message": "User registered successfully"}, format=True), "text/json"]

# API: User Login
@srv.route("/api/login")
def login_user():
    body = req.body
    jsonBody = yaml.loads(body)
    
    username = jsonBody["username"]
    password = jsonBody["password"]
    
    pushWritepad(sqlite)
    
    # Verify credentials
    %login_check = SELECT username, display_name FROM users WHERE username = ${username} AND password = ${password};
    user_data = login_check.fetch()
    
    if user_data == None:
        popWritepad()
        return [str({"success": False, "message": "Invalid credentials"}, format=True), "text/json"]
    
    # Update last login time
    current_time = time.ctime()
    %UPDATE users SET last_login = ${current_time} WHERE username = ${username};
    
    popWritepad()
    
    # Generate a simple session token (in a real app, use proper JWT or secure tokens)
    token = username + "_" + str(time.time())
    
    return [str({
        "success": True, 
        "message": "Login successful", 
        "username": user_data[0],
        "display_name": user_data[1],
        "token": token
    }, format=True), "text/json"]

# API: Get User List (protected endpoint)
@srv.route("/api/users")
def get_users():
    # Check if user is authenticated (check Authorization header in a real app)
    """
    headers = req.all_headers
    if "Authorization" not in headers:
        return [str({"success": False, "message": "Authentication required"}, format=True), "text/json"]
    """
    pushWritepad(sqlite)
    %users_query = SELECT username, display_name, last_login FROM users;
    
    users = []
    row = users_query.fetch()
    
    while row != None:
        users.append({
            "username": row[0],
            "display_name": row[1],
            "last_login": row[2]
        })
        row = users_query.fetch()
    
    popWritepad()
    
    return [str({"success": True, "users": users}, format=True), "text/json"]

# API: Ping endpoint (for testing)
@srv.route("/api/ping")
def ping():
    return [str({"success": True, "message": "Server is running"}, format=True), "text/json"]

# API: Delete User (admin function)
@srv.route("/api/delete_user")
def delete_user():
    body = req.body
    jsonBody = yaml.loads(body)
    
    admin_username = jsonBody["admin_username"]
    admin_password = jsonBody["admin_password"]
    target_username = jsonBody["target_username"]
    
    # Verify admin credentials first
    pushWritepad(sqlite)
    %admin_check = SELECT username FROM users WHERE username = ${admin_username} AND password = ${admin_password};
    admin_exists = admin_check.fetch()
    
    if admin_exists == None:
        popWritepad()
        return [str({"success": False, "message": "Admin authentication failed"}, format=True), "text/json"]
    
    # Don't allow deleting the admin account
    if target_username == "admin":
        popWritepad()
        return [str({"success": False, "message": "Cannot delete admin account"}, format=True), "text/json"]
    
    # Delete the target user
    %DELETE FROM users WHERE username = ${target_username};
    
    popWritepad()
    return [str({"success": True, "message": "User deleted successfully"}, format=True), "text/json"]

# Start the server
print("Server starting on port 8080...")
srv.listen("::", 8080)
