/* wsg_50_sim_driver
 * Copyright (c) 2012, Robotnik Automation, SLL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Robotnik Automation, SLL. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \author Marc Benetó (mbeneto@robotnik.es)
 * \brief WSG-50 sim driver.
 */

#include <rclcpp/rclcpp.hpp>
#include <wsg_50_common/srv/move.hpp>
#include <wsg_50_common/srv/incr.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_srvs/srv/empty.hpp>

#define GRIPPER_MAX_OPEN 110.0
#define GRIPPER_MIN_OPEN 0.0

using namespace std;

rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr vel_pub_r_, vel_pub_l_;
std::shared_ptr<rclcpp::Node> g_node;
double currentOpenning;

void move(double width){
	
		double open = width / 2;
		
		std_msgs::msg::Float64 lCommand, rCommand;
		
		rCommand.data = open/1000;
		lCommand.data = rCommand.data * -1.0;
		
		vel_pub_r_->publish(rCommand);
		vel_pub_l_->publish(lCommand);
		
		currentOpenning = width;
	
}

void moveSrv(const std::shared_ptr<wsg_50_common::srv::Move::Request> req,
             std::shared_ptr<wsg_50_common::srv::Move::Response> res)
{
	if ( req->width >= 0.0 && req->width <= 110.0 ){
  		RCLCPP_INFO(g_node->get_logger(), "Moving to %f position.", req->width);
		move(req->width);
		
	}else if (req->width < 0.0 || req->width > 110.0){
		RCLCPP_ERROR(g_node->get_logger(), "Imposible to move to this position. (Width values: [0.0 - 110.0] ");
		return;
	}

	RCLCPP_INFO(g_node->get_logger(), "Target position reached.");
}

void moveIncrementallySrv(const std::shared_ptr<wsg_50_common::srv::Incr::Request> req,
                          std::shared_ptr<wsg_50_common::srv::Incr::Response> res)
{
				
	if (req->direction == "open"){
		
		float nextWidth = currentOpenning + req->increment;
		
		if ( (currentOpenning < GRIPPER_MAX_OPEN) && nextWidth < GRIPPER_MAX_OPEN ){
			move(nextWidth);
		}else if( nextWidth >= GRIPPER_MAX_OPEN){
			move(nextWidth);
			currentOpenning = GRIPPER_MAX_OPEN;
		}

	}else if (req->direction == "close"){

		float nextWidth = currentOpenning - req->increment;
		
		if ( (currentOpenning > GRIPPER_MIN_OPEN) && nextWidth > GRIPPER_MIN_OPEN ){
			move(nextWidth);
		}else if( nextWidth <= GRIPPER_MIN_OPEN){
			move(nextWidth);
			currentOpenning = GRIPPER_MIN_OPEN;
		}
	}
	
}


void homingSrv(const std::shared_ptr<std_srvs::srv::Empty::Request> req,
               std::shared_ptr<std_srvs::srv::Empty::Response> res)
{
	RCLCPP_INFO(g_node->get_logger(), "Homing...");
	
	move(0.0);
	
	RCLCPP_INFO(g_node->get_logger(), "Home position reached.");
}

void graspSrv(const std::shared_ptr<wsg_50_common::srv::Move::Request> req,
              std::shared_ptr<wsg_50_common::srv::Move::Response> res)
{
	RCLCPP_INFO(g_node->get_logger(), "Grasping...");
	
	// TODO: Increase finger force
	move(0.0);
	
	RCLCPP_INFO(g_node->get_logger(), "Object grasped");
}


int main(int argc, char** argv){
	
	rclcpp::init(argc, argv);
	g_node = std::make_shared<rclcpp::Node>("wsg_50_sim_driver");
	
	std::string vel_pub_l_Topic, vel_pub_r_Topic;
	
	g_node->declare_parameter("vel_pub_l_Topic", "/wsg_50_gl/command");
	g_node->declare_parameter("vel_pub_r_Topic", "/wsg_50_gr/command");
	vel_pub_l_Topic = g_node->get_parameter("vel_pub_l_Topic").as_string();
	vel_pub_r_Topic = g_node->get_parameter("vel_pub_r_Topic").as_string();
	
    currentOpenning = 0.0;
	
	auto moveSS = g_node->create_service<wsg_50_common::srv::Move>("move", moveSrv);
	auto moveIncrementallySS = g_node->create_service<wsg_50_common::srv::Incr>("move_incrementally", moveIncrementallySrv);
	auto homingSS = g_node->create_service<std_srvs::srv::Empty>("homing", homingSrv);
	auto graspSS = g_node->create_service<wsg_50_common::srv::Move>("grasp", graspSrv);
	
	vel_pub_l_ = g_node->create_publisher<std_msgs::msg::Float64>(vel_pub_l_Topic, 1000);
	vel_pub_r_ = g_node->create_publisher<std_msgs::msg::Float64>(vel_pub_r_Topic, 1000);
	
	rclcpp::spin(g_node);
	rclcpp::shutdown();
	return 0;
} 
