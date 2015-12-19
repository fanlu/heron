import tornado.gen
import tornado.web

from heron.tracker.src.python import constants
from heron.tracker.src.python.handlers import BaseHandler

class LogicalPlanHandler(BaseHandler):
  """
  URL - /topologies/logicalplan
  Parameters:
   - dc (required)
   - environ (required)
   - topology (required) name of the requested topology

  The response JSON is a dictionary with all the
  information of logical plan of the topology.
  """
  def initialize(self, tracker):
    self.tracker = tracker

  @tornado.gen.coroutine
  def get(self):
    try:
      dc = self.get_argument_dc()
      environ = self.get_argument_environ()
      topName = self.get_argument_topology()
      topologyInfo = self.tracker.getTopologyInfo(topName, dc, environ)
      logicalPlan = topologyInfo["logical_plan"]
      self.write_success_response(logicalPlan)
    except Exception as e:
      self.write_error_response(e)

