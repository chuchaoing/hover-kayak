/*
 * BHV_Waypoint_Hover
 *        File: BHV_Waypoint.h
 *  Created on: Aug 17, 2014
 *      Author: Josh Leighton
 */

/*
 * This file is modified from the original MOOS-IvP version to
 * reduce overshoot when starting on a new leg.
 */

/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: BHV_Waypoint.h                                       */
/*    DATE: Nov 2004 (original version - many changes since)     */
/*                                                               */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation; either version  */
/* 2 of the License, or (at your option) any later version.      */
/*                                                               */
/* This program is distributed in the hope that it will be       */
/* useful, but WITHOUT ANY WARRANTY; without even the implied    */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the GNU General Public License for more details. */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with this program; if not, write to the Free    */
/* Software Foundation, Inc., 59 Temple Place - Suite 330,       */
/* Boston, MA 02111-1307, USA.                                   */
/*****************************************************************/

#ifndef BHV_WAYPOINT_HEADER
#define BHV_WAYPOINT_HEADER

#include <string>
#include "IvPBehavior.h"
#include "WaypointEngine.h"
#include "XYPoint.h"

class BHV_Waypoint_Hover: public IvPBehavior {
public:
    BHV_Waypoint_Hover(IvPDomain);
    ~BHV_Waypoint_Hover() {};

    bool setParam(std::string, std::string);
    IvPFunction* onRunState();
    BehaviorReport onRunState(std::string);
    void onRunToIdleState();
    void onSetParamComplete();
    void onCompleteState() { postErasables(); }

protected:
    bool updateInfoIn();
    bool setNextWaypoint();
    IvPFunction* buildOF(std::string);

    void postStatusReport();
    void postViewableSegList();
    void postErasableSegList();
    void postErasables();
    void postCycleFlags();
    void postWptFlags(double x, double y);
    void handleVisualHint(std::string);

protected:
    WaypointEngine m_waypoint_engine;

protected:
    // configuration parameters
    double m_cruise_speed;
    bool m_lead_to_start;
    double m_lead_distance;
    double m_lead_damper;
    std::string m_ipf_type;

    // Configurable names of MOOS variables for reports
    std::string m_var_report;
    std::string m_var_index;
    std::string m_var_cyindex;
    std::string m_var_suffix;

    // Var-Data flags for posting when behavior finishes cycle
    std::vector<VarDataPair> m_cycle_flags;
    std::vector<VarDataPair> m_wpt_flags;

    // Visual hints affecting properties of polygons/points
    std::string m_hint_vertex_color;
    std::string m_hint_edge_color;
    double m_hint_vertex_size;
    double m_hint_edge_size;

protected:
    // intermediate or object global variables.
    double m_osv;  // Ownship velocity
    double m_osx;  // Ownship x position
    double m_osy;  // Ownship y position

    XYPoint m_nextpt;
    XYPoint m_trackpt;
    XYPoint m_markpt;

    XYPoint m_prevpt;

    bool m_greedy_tour_pending;

protected:
    // new variables for hovergroup trackpoint modification
    // config
    double m_heading_cutoff;

    //
    double m_nav_heading;

    double m_latestX, m_latestY;

    bool m_mod_is_active, m_mod_is_ending;
    double m_start_time;

    double angleDiff(double a, double b);

    bool m_skip_first_waypoint;


};

#define IVP_EXPORT_FUNCTION

extern "C" {
    IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain)
    {return new BHV_Waypoint_Hover(domain);}
}

#endif

