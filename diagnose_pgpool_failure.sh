#!/bin/bash
# Diagnostic script to identify why pgpool2 service is not starting

echo "=========================================="
echo "pgpool2 Service Failure Diagnosis"
echo "=========================================="
echo ""

echo "1. SERVICE STATUS:"
echo "-----------------"
systemctl status pgpool2 --no-pager | head -15
echo ""

echo "2. ERROR ANALYSIS:"
echo "------------------"
echo "Error from logs:"
journalctl -u pgpool2 -n 5 --no-pager 2>/dev/null | grep -E "(FATAL|DETAIL|ERROR)" || echo "Cannot read logs (needs sudo)"
echo ""

echo "3. ROOT CAUSE:"
echo "--------------"
echo "The error shows:"
echo "  FATAL: syntax error in configuration file"
echo "  DETAIL: parse error at line 128 'etc/pgpool2/pool_passwd' token = 5"
echo ""
echo "This means:"
echo "  ✗ Line 128 in /etc/pgpool2/pgpool.conf has a syntax error"
echo "  ✗ The path 'etc/pgpool2/pool_passwd' is missing the leading '/'"
echo "  ✗ OR the path is missing quotes around it"
echo ""

echo "4. EXPECTED CONFIGURATION:"
echo "--------------------------"
echo "The pool_passwd line should look like:"
echo "  pool_passwd = '/etc/pgpool2/pool_passwd'"
echo ""
echo "NOT like:"
echo "  pool_passwd = etc/pgpool2/pool_passwd    (missing / and quotes)"
echo "  pool_passwd = /etc/pgpool2/pool_passwd   (missing quotes)"
echo ""

echo "5. FIX COMMANDS (run with sudo):"
echo "---------------------------------"
echo ""
echo "Option A: Fix the syntax (recommended):"
echo "  sudo sed -n '125,135p' /etc/pgpool2/pgpool.conf"
echo "  sudo sed -i \"s|^pool_passwd =.*|pool_passwd = '/etc/pgpool2/pool_passwd'|\" /etc/pgpool2/pgpool.conf"
echo "  sudo grep '^pool_passwd' /etc/pgpool2/pgpool.conf"
echo "  sudo systemctl restart pgpool2"
echo ""
echo "Option B: Temporarily disable pool_passwd:"
echo "  sudo sed -i 's/^pool_passwd =/#pool_passwd =/' /etc/pgpool2/pgpool.conf"
echo "  sudo systemctl restart pgpool2"
echo ""
echo "Option C: Test configuration before restarting:"
echo "  sudo pgpool -n -f /etc/pgpool2/pgpool.conf -F /etc/pgpool2/pcp.conf -a /etc/pgpool2/pool_hba.conf 2>&1 | head -20"
echo ""

echo "6. VERIFICATION STEPS:"
echo "---------------------"
echo "After fixing, verify:"
echo "  1. sudo systemctl status pgpool2"
echo "  2. sudo netstat -tlnp | grep 9999  (should show pgpool listening)"
echo "  3. psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c 'SELECT 1;'"
echo ""

echo "=========================================="
echo "Summary: Configuration syntax error at line 128"
echo "Fix: Correct the pool_passwd path format"
echo "=========================================="







