# Simple XLang WebServer

A lightweight authentication-based web server built with XLang (.x) and a single-page HTML/JavaScript frontend.

## Overview

This project demonstrates a basic web application with user authentication, user management, and session handling. It uses XLang on the backend with SQLite for data persistence and a straightforward HTML/CSS/JavaScript frontend.

## Features

- **User Authentication System**
  - Login and logout functionality
  - User registration with form validation
  - Session persistence using localStorage
  - Password verification

- **User Management**
  - View list of all registered users
  - See last login timestamps
  - Admin can delete users (except the admin account)

- **Simple and Clean UI**
  - Responsive design
  - Modal dialogs for login and registration
  - Dynamic content based on authentication status

## Technical Details

### Backend (server.x)

The server is built using XLang with the following components:

- **Database**: SQLite database for user storage
- **API Endpoints**:
  - `/api/register` - Create new user accounts
  - `/api/login` - Authenticate users and create sessions
  - `/api/users` - Retrieve list of all users (protected)
  - `/api/ping` - Check server status
  - `/api/delete_user` - Admin endpoint to remove users
- **Authentication**: Simple token-based authentication system

### Frontend (index.html)

A single-page application with:

- Clean, responsive UI using vanilla CSS
- No external dependencies (pure JavaScript)
- Modular HTML structure with dynamic content loading
- Local storage for persisting authentication between sessions

## Installation

1. Ensure you have XLang installed on your system
2. Clone this repository:
   ```
   git clone https://github.com/yourusername/simple-xlang-webserver.git
   cd simple-xlang-webserver
   ```

## Running the Application

1. Start the XLang server:
   ```
   xlang server.x
   ```
2. Open your browser and navigate to:
   ```
   http://localhost:8080
   ```

## Default Admin Account

The system comes with a pre-configured admin account:
- **Username**: admin
- **Password**: admin123

## Development Notes

### Database Structure

The application uses a single `users` table with the following schema:
- `username` (TEXT, PRIMARY KEY)
- `password` (TEXT)
- `display_name` (TEXT)
- `last_login` (TEXT)

### Security Considerations

This is a demonstration project and includes several simplified implementations that would need enhancement for production use:

- Passwords are stored in plain text (should use proper hashing)
- Basic token authentication (should use JWT or similar)
- No HTTPS implementation
- Minimal input validation

## Extending the Application

Here are some ways you could extend this project:

1. Add password hashing for security
2. Implement proper JWT authentication
3. Add user roles and permissions
4. Create additional protected resources
5. Add password reset functionality
6. Implement HTTPS support

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
