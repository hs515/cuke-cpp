Feature: test

Scenario: Process a JSON request
  Given the following JSON payload:
    """
    {
      "user": "testuser",
      "action": "create",
      "data": {
        "id": 123
      }
    }
    """

