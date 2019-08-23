/*
 * Copyright (C) 2019 Logical Clocks AB
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef EPIPE_METRICSPROVIDER_H
#define EPIPE_METRICSPROVIDER_H
class MetricsProvider{
public:
  virtual std::string getMetrics() const = 0;
};

class MetricsProviders : public MetricsProvider{
public:
  MetricsProviders(const std::vector<MetricsProvider*> providers) :
  mProviders(providers){}

  std::string getMetrics() const override {
    std::stringstream out;
    for(auto m : mProviders){
      out << m->getMetrics() << std::endl;
    }
    return out.str();
  }

private:
  const std::vector<MetricsProvider*>  mProviders;
};
#endif //EPIPE_METRICSPROVIDER_H