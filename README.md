🕒 TimePunch - Smart Time Tracking System

TimePunch is a lightweight, customizable time-tracking application inspired by factory-style punch-in systems. Built in C for Windows and Linux (Raspberry Pi 4 compatible), it offers accurate, local presence tracking with full daily/hourly reporting.
🔧 Features

    Always-on fullscreen interface

    Numeric keypad input + welcome/goodbye messages

    Automatic user ID generation and CSV logging

    Two work shifts per day (4 timestamps)

    Daily, weekly, and monthly report generation

    Admin code (999) with full user management (add/remove/view users)

    Directory-based structure: one folder per user, subfolders per month

    Input validation and automatic time checks

    Logs anomalies and manual changes

🗂 File Structure

users/
  001/
    2024-09/
      2024-09-01.csv
      ...
users.txt
error_log.txt

📊 CSV Format Example

Each CSV includes:

    Entry & exit time for each shift

    Calculated hours per shift

    Total daily hours

    Blank lines for visual clarity

    (Future) Conditional coloring for quick status review

⚠️ Missing Entries Management

    Auto-detection of incomplete records

    Admin correction tools

    Retroactive logging

    Flag system for invalid data

    Input/output validation to avoid logical errors

🔐 Data Integrity

    Admin access required for edits

    Backup-friendly structure

    API-ready (for Google Sheets, Excel, etc.)
