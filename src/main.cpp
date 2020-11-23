#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	std::shared_ptr<OdomChassisController> drive =
		ChassisControllerBuilder()
				.withMotors(
						1,  // Top left
						-2, // Top right (reversed)
						-3, // Bottom right (reversed)
						4   // Bottom left
				)
				// Green gearset, 4 inch wheel diameter, 11.5 inch wheelbase
				.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
				.withGains(
					{0.001, 0, 0.0001}, // Distance controller gains
					{0.001, 0, 0.0001}, // Turn controller gains
					{0.001, 0, 0.0001}  // Angle controller gains (helps drive straight)
			)
			.withOdometry()
			.buildOdometry();

			drive->setState({0_in, 0_in, 0_deg});
			drive->driveToPoint({1_ft, 1_ft});

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
		Controller controller;

		std::shared_ptr<ChassisController> drive =
			ChassisControllerBuilder()
					.withMotors(
							1,  // Top left
							-2, // Top right (reversed)
							-3, // Bottom right (reversed)
							4   // Bottom left
					)
					// Green gearset, 4 inch wheel diameter, 11.5 inch wheelbase
					.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
					.withGains(
						{0.001, 0, 0.0001}, // Distance controller gains
						{0.001, 0, 0.0001}, // Turn controller gains
						{0.001, 0, 0.0001}  // Angle controller gains (helps drive straight)
				)
				.build();




				auto xModel = std::dynamic_pointer_cast<XDriveModel>(drive->getModel());

				while(true){
					//XDrive go here
					xModel->xArcade(controller.getAnalog(ControllerAnalog::leftY),
                        controller.getAnalog(ControllerAnalog::leftX),
												controller.getAnalog(ControllerAnalog::rightX));

				pros::delay(20);

			}




}
