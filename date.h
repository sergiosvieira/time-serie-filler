#ifndef DATE_H
#define DATE_H

#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include "fmt/include/fmt/format.h"

using std::map, std::vector;
using ushort = unsigned short;
using uint = unsigned int;
using Date = struct {ushort day = 1; ushort month = 1; uint year = 0;};
enum class Month
{
    JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
};
using DaysMap = map<Month, ushort>;
using DaysVector = vector<Month>;
using VectorInt = vector<uint>;
static const DaysVector daysVector =
{
    Month::JAN,
    Month::FEB,
    Month::MAR,
    Month::APR,
    Month::MAY,
    Month::JUN,
    Month::JUL,
    Month::AUG,
    Month::SEP,
    Month::OCT,
    Month::NOV,
    Month::DEC
};
static const DaysMap daysMap =
{
    {Month::JAN, 31},
    {Month::FEB, 28},
    {Month::MAR, 31},
    {Month::APR, 30},
    {Month::MAY, 31},
    {Month::JUN, 30},
    {Month::JUL, 31},
    {Month::AUG, 31},
    {Month::SEP, 30},
    {Month::OCT, 31},
    {Month::NOV, 30},
    {Month::DEC, 31}
};

VectorInt daysNoLeap = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
VectorInt daysLeap = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

Date increment_month(const Date& date);
Date increment_year(const Date& date);
bool is_leap_year(const Date& date);

template <>
struct fmt::formatter<Date> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
  template <typename FormatContext>
  auto format(const Date& d, FormatContext& ctx) {
    return format_to(ctx.out(), "{}{:02}{:02}", d.year, d.month, d.day);
  }
};

Date increment_day(const Date& date)
{
    Date result = date;
    Month month = daysVector[date.month - 1];
    auto it = daysMap.find(month);
    ushort days = it->second;
    if (is_leap_year(date)
            && result.month == 2) days = 29;
    if (date.day + 1 <= days)
    {
        result.day++;
    }
    else
    {
        result.day = 1;
        result = increment_month(result);
    }
    return result;
}

Date increment_month(const Date& date)
{
    Date result = date;
    if (date.month + 1 < 13)
    {
        result.month++;
    }
    else
    {
        result.month = 1;
        result.year++;
    }
    return result;
}

Date increment_year(const Date& date)
{
    Date result = date;
    ++result.year;
    return result;
}

std::string date_str(const Date& date)
{
    std::ostringstream ss;
    ss << date.year;
    ss << std::setw(2) << std::setfill('0') << date.month;
    ss << std::setw(2) << std::setfill('0') << date.day;
    return ss.str();
}

bool is_leap_year(const Date& date)
{
   bool result = false;
   if (date.year % 4 == 0)
   {
      if (date.year % 100 == 0)
      {
         if (date.year % 400 == 0)
         {
            result = true;
         }
      }
      else result = true;
   }
   return result;
}

/**
 * @brief from_str - Valid Format: 19000101 (4 digits to year, 2 digits to month and 2 digits to day)
 * @param str
 * @return
 */
Date from_str(const std::string& str)
{
    Date result;
    if (str.size() != 8) return result;
    uint year = static_cast<uint>(std::atoi(str.substr(0, 4).c_str()));
    ushort month = static_cast<ushort>(std::atoi(str.substr(4, 2).c_str()));
    ushort day = static_cast<ushort>(std::atoi(str.substr(6, 2).c_str()));
    result.year = year;
    result.month = month;
    result.day = day;
    return result;
}

uint days_year(const Date& date)
{
    return is_leap_year(date) ? 366 : 365;
}

uint remaining_days(const Date& date)
{
    uint daysFirst = days_year(date);
    uint accDays = is_leap_year(date)
            ? daysLeap[date.month - 1]
            : daysNoLeap[date.month - 1];
    accDays += date.day;
    return daysFirst - accDays;
}

uint diff_date(const Date& first, const Date& last)
{
    if (last.year < first.year) return 0;
    if (first.year == last.year
            && last.month < first.month) return 0;
    if (first.year == last.year
            && first.month == last.month
            && last.day < first.day) return 0;
    uint remainingFirst = remaining_days(first);
    uint remainingLast = remaining_days(last);
    if (first.year == last.year)
        return remainingLast - remainingFirst;
    uint days = 0;
    for (uint i = first.year + 1; i < last.year; ++i)
    {
        days += days_year({1, 1, i});
    }
    return remainingFirst + days + days_year({1, 1, last.year}) - remainingLast;
}

#endif // DATE_H
