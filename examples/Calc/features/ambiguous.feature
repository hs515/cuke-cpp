# language: en
@ftag
Feature: Ambiguous Addition
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the sum of two numbers

  @stag
  Scenario Outline: Add two numbers
    Given I have entered an integer number 2 into the calculator
    And I have entered an integer number 8 into the calculator
    When I press add
    Then the result should be 10 on the screen
