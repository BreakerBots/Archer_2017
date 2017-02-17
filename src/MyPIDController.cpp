/*
 * MyPIDController.cpp
 *
 *  Created on: Feb 17, 2017
 *      Author: DS_2016
 */


#include "PIDController.h"
#include "MyPIDController.h"

MyPIDController::MyPIDController(double p, double i, double d, PIDSource *input, PIDOutput *output):
	PIDController (p,i,d,input,output)
{
}

void PIDController::Calculate (){
	bool enabled;
	  PIDSource* pidInput;
	  {
	    std::lock_guard<priority_recursive_mutex> lock(m_mutex);
	    if (m_pidInput == 0) return;
	    if (m_pidOutput == 0) return;
	    enabled = m_enabled;
	    pidInput = m_pidInput;
	  }

	  if (enabled) {
	    double input = pidInput->PIDGet();
	    double result;
	    PIDOutput* pidOutput;

	    {
	      std::lock_guard<priority_recursive_mutex> sync(m_mutex);
	      m_error = m_setpoint - input;
	      if (m_continuous) {
	        if (std::fabs(m_error) > (m_maximumInput - m_minimumInput) / 2) {
	          if (m_error > 0) {
	            m_error = m_error - (m_maximumInput - m_minimumInput);
	          } else {
	            m_error = m_error + (m_maximumInput - m_minimumInput);
	          }
	        }
	      }

	      if (m_pidInput->GetPIDSourceType() == PIDSourceType::kRate) {
	        if (m_P != 0) {
	          double potentialPGain = (m_totalError + m_error) * m_P;
	          if (potentialPGain < m_maximumOutput) {
	            if (potentialPGain > m_minimumOutput) {
	              m_totalError += m_error;
	            } else {
	              m_totalError = m_minimumOutput / m_P;
	            }
	          } else {
	            m_totalError = m_maximumOutput / m_P;
	          }
	        }

	        m_result = m_D * m_error + m_P * m_totalError + CalculateFeedForward();
	      } else {
	        if (m_I != 0) {
	          double potentialIGain = (m_totalError + m_error) * m_I;
	          if (potentialIGain < m_maximumOutput) {
	            if (potentialIGain > m_minimumOutput) {
	              m_totalError += m_error;
	            } else {
	              m_totalError = m_minimumOutput / m_I;
	            }
	          } else {
	            m_totalError = m_maximumOutput / m_I;
	          }
	        }
/*
 * I, Henry Loh, am overriding this method to allow for an Integral Zone to be
 * monitored by the PIDController.
 */
	        if (std::abs(m_error) <= MyPIDController::IZone){
	        	m_result = m_P * m_error + m_I * m_totalError +
	                   m_D * (m_error - m_prevError) + CalculateFeedForward();
	        	printf("Using    I Value\n");
	        } else {
	        	m_result = m_P * m_error + /* No I Values */
	        			m_D * (m_error - m_prevError) + CalculateFeedForward();
	        	printf("Ignoring I Value\n");
	        }
	      }
	      m_prevError = m_error;

	      if (m_result > m_maximumOutput)
	        m_result = m_maximumOutput;
	      else if (m_result < m_minimumOutput)
	        m_result = m_minimumOutput;

	      pidOutput = m_pidOutput;
	      result = m_result;
	    }

	    pidOutput->PIDWrite(result);
	}
}

